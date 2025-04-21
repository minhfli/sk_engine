#include "Box2dTest.h"

#include <GLM/glm.hpp>
#include <box2d/box2d.h>

#include <iostream>
#include <vector>

#include <sk_engine/Core/sk_time.h>
#include <sk_engine/Graphics/2D_Renderer.h>


namespace b2_test {

    b2World physic_world(b2Vec2(0, -10));
    std::vector<b2Body*> m_bodies;
    int dynamic_body_count = 10;

    void Setup() {

        b2BodyDef def;
        b2PolygonShape shape;

        // ground box
        shape.SetAsBox(30, 5);
        def.type = b2_staticBody;
        def.position = b2Vec2(0, -5);

        b2Body* body = physic_world.CreateBody(&def);
        body->CreateFixture(&shape, 0);
        m_bodies.emplace_back(body);

        shape.SetAsBox(1, 1);
        // dynamic boxes
        for (int i = 1;i <= dynamic_body_count; i++) {
            def.type = b2_dynamicBody;
            def.position = b2Vec2(0, i);

            b2FixtureDef fixture;
            fixture.density = 1;
            fixture.friction = .3f;
            fixture.shape = &shape;

            body = physic_world.CreateBody(&def);
            body->CreateFixture(&fixture);

            m_bodies.emplace_back(body);
        }
        std::cout << "box2d setup done\n";
    }

    void Update() {
        physic_world.Step(sk_time::fixed_delta_time, 8, 3);

    }


    void DrawBox(b2PolygonShape shape, b2Body* body) {

        int n = shape.m_count;
        glm::vec2 points[10];

        for (int i = 1; i <= n; i++) {
            b2Vec2 p = body->GetWorldPoint(shape.m_vertices[i - 1]);
            points[i] = { p.x,p.y };
        }
        for (int i = 2; i <= n; i++) {
            sk_graphic::Renderer2D_AddLine({ points[i],0 }, points[i - 1] - points[i]);
        }
        sk_graphic::Renderer2D_AddLine({ points[1],0 }, points[n] - points[1]);

    }
    void Draw() {
        for (auto body : m_bodies) {
            b2Vec2 pos = body->GetPosition();
            float angle = body->GetAngle();
            b2Fixture* m_fixtures = body->GetFixtureList();
            body->GetFixtureList();
            for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext()) {
                b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();

                DrawBox(*polygonShape, body);
            }
        }
    }
}