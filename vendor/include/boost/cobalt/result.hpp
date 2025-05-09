// Copyright (c) 2023 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_COBALT_RESULT_HPP
#define BOOST_COBALT_RESULT_HPP

#include <boost/cobalt/concepts.hpp>

#include <boost/core/no_exceptions_support.hpp>
#include <boost/system/result.hpp>

namespace boost::cobalt
{


namespace detail
{

template<typename T>
concept result_error =
  requires (const T & t, const source_location & loc)
  {
    system::throw_exception_from_error(t, loc);
  }
  || // ADL
  requires (const T & t, const source_location & loc)
  {
    throw_exception_from_error(t, loc);
  }
;

}

inline constexpr auto interpret_as_result(std::tuple<> &&)
{
  return system::result<void>();
}

template<typename Arg>
auto interpret_as_result(std::tuple<Arg> && args)
{
  if constexpr (detail::result_error<Arg>)
  {
    if (std::get<0>(args))
      return system::result<void, Arg>(system::in_place_error, std::get<0>(args));
    else
      return system::result<void, Arg>(system::in_place_value);
  }

  else
    return system::result<Arg>(std::move(std::get<0>(args)));
}

template<typename First, typename ... Args>
  requires (!detail::result_error<First> && sizeof...(Args) > 0u)
auto interpret_as_result(std::tuple<First, Args...> && args) -> system::result<std::tuple<First, Args...>>
{
  return std::move(args);
}

template<detail::result_error Error, typename  ... Args>
  requires (sizeof...(Args) > 1u)
auto interpret_as_result(std::tuple<Error, Args...> && args) -> system::result<std::tuple<Args...>, Error>
{
  if (std::get<0>(args))
    return {system::in_place_error, std::move(std::get<0>(args))};
  return {
      system::in_place_value,
      std::apply([](auto, auto && ... rest) {return std::make_tuple(std::move(rest)...);}, std::move(args))
  };
}

template<detail::result_error Error, typename Arg>
auto interpret_as_result(std::tuple<Error, Arg> && args) -> system::result<Arg, Error>
{
  if (std::get<0>(args))
    return {system::in_place_error, std::get<0>(args)};

  return {system::in_place_value, std::get<1>(std::move(args))};
}

struct as_result_tag {};
struct as_tuple_tag {};

template<awaitable_type Aw>
struct as_result_t
{
  as_result_t(Aw && aw) : aw_(std::forward<Aw>(aw)) {}

  template<typename Aw_>
    requires requires (Aw_ && aw) {{std::forward<Aw_>(aw).operator co_await()} -> awaitable_type;}
  as_result_t(Aw_ && aw) : aw_(std::forward<Aw_>(aw).operator co_await()) {}

  template<typename Aw_>
    requires requires (Aw_ && aw) {{operator co_await(std::forward<Aw_>(aw))} -> awaitable_type;}
  as_result_t(Aw_ && aw) : aw_(operator co_await(std::forward<Aw_>(aw))) {}

  bool await_ready() { return aw_.await_ready();}
  template<typename T>
  auto await_suspend(std::coroutine_handle<T> h) { return aw_.await_suspend(h);}

  auto await_resume()
  {
    if constexpr (requires {aw_.await_resume(as_result_tag{});})
      return aw_.await_resume(as_result_tag{});
    else
    {
      using type = decltype(aw_.await_resume());
      if constexpr (std::is_void_v<type>)
      {
        using res_t = system::result<type, std::exception_ptr>;
        BOOST_TRY
        {
          aw_.await_resume();
          return res_t{system::in_place_value};
        }
        BOOST_CATCH (...)
        {
          return res_t{system::in_place_error, std::current_exception()};
        }
        BOOST_CATCH_END
      }
      else
      {
        using res_t = system::result<type, std::exception_ptr>;
        BOOST_TRY
        {
          return res_t{system::in_place_value, aw_.await_resume()};
        }
        BOOST_CATCH (...)
        {
          return res_t{system::in_place_error, std::current_exception()};
        }
        BOOST_CATCH_END
      }
    }
  }
 private:
  Aw aw_;
};


template<awaitable_type Aw>
as_result_t(Aw &&) -> as_result_t<Aw>;

template<typename Aw_>
  requires requires (Aw_ && aw) {{std::forward<Aw_>(aw).operator co_await()} -> awaitable_type;}
as_result_t(Aw_ && aw) -> as_result_t<decltype(std::forward<Aw_>(aw).operator co_await())>;

template<typename Aw_>
  requires requires (Aw_ && aw) {{operator co_await(std::forward<Aw_>(aw))} -> awaitable_type;}
as_result_t(Aw_ && aw) -> as_result_t<decltype(operator co_await(std::forward<Aw_>(aw)))>;

template<awaitable_type Aw>
auto as_result(Aw && aw) -> as_result_t<Aw>
{
  return as_result_t<Aw>(std::forward<Aw>(aw));
}

template<typename Aw>
  requires requires (Aw && aw)
  {
    {std::forward<Aw>(aw).operator co_await()} -> awaitable_type;
  }
auto as_result(Aw && aw)
{
  struct lazy_tuple
  {
    Aw aw;
    auto operator co_await ()
    {
      return as_result_t(std::forward<Aw>(aw));
    }
  };
  return lazy_tuple{std::forward<Aw>(aw)};
}

template<typename Aw>
  requires requires (Aw && aw)
  {
    {operator co_await(std::forward<Aw>(aw))} -> awaitable_type;
  }
auto as_result(Aw && aw)
{
  struct lazy_tuple
  {
    Aw aw;
    auto operator co_await ()
    {
      return as_result_t(std::forward<Aw>(aw));
    }
  };
  return lazy_tuple{std::forward<Aw>(aw)};
}



template<awaitable Aw>
struct as_tuple_t
{
  as_tuple_t(Aw && aw) : aw_(std::forward<Aw>(aw)) {}

  template<typename Aw_>
    requires requires (Aw_ && aw) {{std::forward<Aw_>(aw).operator co_await()} -> awaitable_type;}
  as_tuple_t(Aw_ && aw) : aw_(std::forward<Aw_>(aw).operator co_await()) {}

  template<typename Aw_>
    requires requires (Aw_ && aw) {{operator co_await(std::forward<Aw_>(aw))} -> awaitable_type;}
  as_tuple_t(Aw_ && aw) : aw_(operator co_await(std::forward<Aw_>(aw))) {}


  bool await_ready() { return aw_.await_ready();}
  template<typename T>
  auto await_suspend(std::coroutine_handle<T> h) { return aw_.await_suspend(h);}

  auto await_resume()
  {
    using type = decltype(aw_.await_resume());
    if constexpr (requires {aw_.await_resume(as_tuple_tag{});})
      return aw_.await_resume(as_tuple_tag{});
    else if constexpr (noexcept(aw_.await_resume()))
    {
      if constexpr (std::is_void_v<type>)
      {
        aw_.await_resume();
        return std::make_tuple();
      }
      else
        return std::make_tuple(aw_.await_resume());

    }
    else
    {
      if constexpr (std::is_void_v<type>)
      {
        BOOST_TRY
        {
          aw_.await_resume();
          return std::make_tuple(std::exception_ptr());
        }
        BOOST_CATCH (...)
        {
          return std::make_tuple(std::current_exception());
        }
        BOOST_CATCH_END
      }
      else
      {
        BOOST_TRY
        {
          return make_tuple_(std::exception_ptr(), aw_.await_resume());
        }
        BOOST_CATCH (...)
        {
          return make_tuple_(std::current_exception(), type());
        }
        BOOST_CATCH_END
      }
    }
  }
 private:
  template<typename ... Args>
  std::tuple<std::exception_ptr, Args...> make_tuple_(std::exception_ptr ep, std::tuple<Args...> && tup)
  {
    return std::apply(
        [&](auto ... args)
        {
          return std::make_tuple(std::move(ep), std::move(args)...);
        }, std::move(tup));
  }

  template<typename Arg>
  std::tuple<std::exception_ptr, Arg> make_tuple_(std::exception_ptr ep, Arg && arg)
  {
    return std::make_tuple(std::move(ep), std::move(arg));
  }

private:

  Aw aw_;
};


template<awaitable_type Aw>
as_tuple_t(Aw &&) -> as_tuple_t<Aw>;

template<typename Aw_>
  requires requires (Aw_ && aw) {{std::forward<Aw_>(aw).operator co_await()} -> awaitable_type;}
as_tuple_t(Aw_ && aw) -> as_tuple_t<decltype(std::forward<Aw_>(aw).operator co_await())>;

template<typename Aw_>
  requires requires (Aw_ && aw) {{operator co_await(std::forward<Aw_>(aw))} -> awaitable_type;}
as_tuple_t(Aw_ && aw) -> as_tuple_t<decltype(operator co_await(std::forward<Aw_>(aw)))>;


template<awaitable_type Aw>
auto as_tuple(Aw && aw) -> as_tuple_t<Aw>
{
  return as_tuple_t<Aw>(std::forward<Aw>(aw));
}

template<typename Aw>
  requires requires (Aw && aw)
  {
    {std::forward<Aw>(aw).operator co_await()} -> awaitable_type;
  }
auto as_tuple(Aw && aw)
{
  struct lazy_tuple
  {
    Aw aw;
    auto operator co_await ()
    {
      return as_tuple_t(std::forward<Aw>(aw));
    }
  };
  return lazy_tuple{std::forward<Aw>(aw)};
}

template<typename Aw>
  requires requires (Aw && aw)
  {
    {operator co_await(std::forward<Aw>(aw))} -> awaitable_type;
  }
auto as_tuple(Aw && aw)
{
  struct lazy_tuple
  {
    Aw aw;
    auto operator co_await ()
    {
      return as_tuple_t(std::forward<Aw>(aw));
    }
  };
  return lazy_tuple{std::forward<Aw>(aw)};
}


}

#endif //BOOST_COBALT_RESULT_HPP
