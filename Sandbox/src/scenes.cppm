module;

#include <stdlib.h>

export module scenes;

import std;

import BridgeMaker;

using namespace bm;
using namespace bm::gfx;

namespace scene
{


    export class StaticPrimitivesScene : public bm::gfx::Scene
    {
    public:

        static Traits<StaticPrimitivesScene>::Ptr make()
        {
            return std::make_unique<StaticPrimitivesScene>();
        }

        StaticPrimitivesScene()
            : Scene("Static Primitives Scene")
        {
            using namespace bm::gfx;

            // -----------------------------------------------------------
            // Create light (white sphere will follow this)
            // -----------------------------------------------------------
            auto light = Light::make();
            light->setColor({ 1.f, 1.f, 1.f });
            light->setPosition({ 5.f, 5.f, 0.f });
            addLight(std::move(light));

            // -----------------------------------------------------------
            // Ground
            // -----------------------------------------------------------
            {
                auto ground = Object::make<Square>();
                ground->setScale({ 20.f, 1.f, 20.f });
                ground->setColor({ 0.2f, 0.25f, 0.3f });
                ground->apply();
                addObject(std::move(ground));
            }

            // -----------------------------------------------------------
            // STATIC PRIMITIVES
            // -----------------------------------------------------------

            // Cube
            {
                auto obj = Object::make<Cube>();
                obj->setPosition({ -4.f, 1.f, -4.f });
                obj->setScale({ 2.f, 2.f, 2.f });
                obj->setColor({ 0.7f, 0.3f, 0.3f });
                obj->apply();
                addObject(std::move(obj));
            }

            // Sphere
            {
                auto obj = Object::make<Sphere>();
                obj->setPosition({ 4.f, 1.1f, -4.f });
                obj->setScale({ 1.2f, 1.2f, 1.2f });
                obj->setColor({ 0.3f, 0.7f, 0.3f });
                obj->apply();
                addObject(std::move(obj));
            }

            // Cylinder
            {
                auto obj = Object::make<Cylinder>();
                obj->setPosition({ -4.f, 1.f, 4.f });
                obj->setScale({ 1.f, 2.f, 1.f });
                obj->setColor({ 0.3f, 0.3f, 0.7f });
                obj->apply();
                addObject(std::move(obj));
            }

            // Cone
            {
                auto obj = Object::make<Cone>();
                obj->setPosition({ 4.f, 1.f, 4.f });
                obj->setScale({ 1.f, 2.f, 1.f });
                obj->setColor({ 0.7f, 0.7f, 0.2f });
                obj->apply();
                addObject(std::move(obj));
            }

            // Pyramid
            {
                auto obj = Object::make<Pyramid>();
                obj->setPosition({ 0.f, 1.f, 0.f });
                obj->setScale({ 2.0f, 2.0f, 2.0f });
                obj->setColor({ 0.8f, 0.5f, 0.2f });
                obj->apply();
                addObject(std::move(obj));
            }

            // -----------------------------------------------------------
            // White sphere to visualize LIGHT position
            // -----------------------------------------------------------
            {
                auto bulb = Object::make<Sphere>();
                bulb->setScale({ 0.3f, 0.3f, 0.3f });
                bulb->setColor({ 1.f, 1.f, 1.f });
                bulb->apply();

                m_lightSphere = bulb.get();
                addObject(std::move(bulb));
            }
        }

        // -----------------------------------------------------------
        // ANIMATION: Light moves in a circle, sphere follows it
        // -----------------------------------------------------------
        void onUpdate(float dt) override
        {
            m_time += dt;

            animateLight();
        }

    private:

        float m_time = 0.f;
        bm::gfx::Object* m_lightSphere = nullptr;

        void animateLight()
        {
            auto& lights = getLights();
            if (lights.empty()) return;

            // Light follows circle around center
            float radius = 6.f;

            float x = radius * std::cos(m_time * 0.6f);
            float z = radius * std::sin(m_time * 0.6f);
            float y = 4.f + std::sin(m_time * 0.8f) * 1.2f;

            glm::vec3 pos{ x, y, z };

            // Update actual light
            lights[0]->setPosition(pos);

            // Move white sphere to indicate light position
            if (m_lightSphere)
            {
                m_lightSphere->setPosition(pos);
                m_lightSphere->apply();
            }
        }
    };



    export class AnimatedRandomTestScene : public bm::gfx::Scene
    {
    public:

        static Traits<AnimatedRandomTestScene>::Ptr make(int count = 50)
        {
            return std::make_unique<AnimatedRandomTestScene>(count);
        }

        AnimatedRandomTestScene(int objectCount = 50)
            : Scene("Random Stress Test"), m_objectCount(objectCount)
        {
            using namespace bm::gfx;

            // ---------------------------
            // Create single animated light
            // ---------------------------
            auto light = Light::make();
            light->setColor({ 1.0f, 1.0f, 1.0f });
            light->setPosition({ 5.0f, 8.0f, 5.0f });
            addLight(std::move(light));

            std::mt19937 rng{ std::random_device{}() };
            std::uniform_real_distribution<float> distPos(-20.f, 20.f);
            std::uniform_real_distribution<float> distY(0.0f, 5.0f);
            std::uniform_real_distribution<float> distScale(0.2f, 3.0f);
            std::uniform_real_distribution<float> distColor(0.2f, 1.0f);
            std::uniform_real_distribution<float> distSpin(-50.f, 50.f);
            std::uniform_real_distribution<float> distBob(0.5f, 2.5f);
            std::uniform_real_distribution<float> distShape(0.f, 7.f);

            // --------------------------------------
            // Create bunch of RANDOM primitives
            // --------------------------------------
            m_objects.reserve(objectCount);

            for (int i = 0; i < objectCount; ++i)
            {
                // Pick a random shape
                float r = distShape(rng);
                bm::Traits<bm::gfx::Object>::Ptr obj;

                if (r < 1.f)      obj = bm::gfx::Object::make<bm::gfx::Cube>();
                else if (r < 2.f) obj = bm::gfx::Object::make<bm::gfx::Sphere>();
                else if (r < 3.f) obj = bm::gfx::Object::make<bm::gfx::Cylinder>();
                else if (r < 4.f) obj = bm::gfx::Object::make<bm::gfx::Cone>();
                else if (r < 5.f) obj = bm::gfx::Object::make<bm::gfx::Pyramid>();
                else if (r < 6.f) obj = bm::gfx::Object::make<bm::gfx::Triangle>();
                else              obj = bm::gfx::Object::make<bm::gfx::Square>();

                glm::vec3 pos{
                    distPos(rng),
                    distY(rng),
                    distPos(rng)
                };

                glm::vec3 scale{
                    distScale(rng),
                    distScale(rng),
                    distScale(rng)
                };

                glm::vec3 color{
                    distColor(rng),
                    distColor(rng),
                    distColor(rng)
                };

                float spinSpeed = distSpin(rng);  // deg/sec
                float bobSpeed = distBob(rng);    // Hz
                float bobHeight = distBob(rng);   // amplitude

                obj->setPosition(pos);
                obj->setScale(scale);
                obj->setColor(color);
                obj->apply();

                // Save animation data
                m_objects.push_back({
                    obj.get(),
                    pos,
                    scale,
                    spinSpeed,
                    bobSpeed,
                    bobHeight,
                    float(i) * 0.5f // phase offset
                    });

                addObject(std::move(obj));
            }
        }

        // -------------------------------------------------
        // UPDATE
        // -------------------------------------------------
        void onUpdate(float dt) override
        {
            m_time += dt;

            animateLight();
            animateObjects(dt);
        }

    private:

        struct AnimatedObject
        {
            bm::gfx::Object* obj;
            glm::vec3 basePos;
            glm::vec3 baseScale;

            float spinSpeed;   // degrees per second
            float bobSpeed;    // Hz
            float bobHeight;   // amplitude
            float phaseOffset; // phase difference
        };

        std::vector<AnimatedObject> m_objects;
        float m_time = 0.f;
        int m_objectCount = 0;

        // -------------------------------------------------
        // LIGHT ANIMATION
        // -------------------------------------------------
        void animateLight()
        {
            auto& lights = getLights();
            if (lights.empty()) return;

            float r = 25.0f;

            float x = r * cos(m_time * 0.6f);
            float z = r * sin(m_time * 0.6f);
            float y = 8.0f + sin(m_time * 1.3f) * 2.0f;

            lights[0]->setPosition({ x, y, z });
        }

        // -------------------------------------------------
        // OBJECTS ANIMATION
        // -------------------------------------------------
        void animateObjects(float dt)
        {
            for (auto& anim : m_objects)
            {
                float t = m_time + anim.phaseOffset;

                // bobbing up/down
                glm::vec3 pos = anim.basePos;
                pos.y += sin(t * anim.bobSpeed * 3.14159f * 2.0f) * anim.bobHeight;

                // rotation around Y
                glm::vec3 rot{ 0.0f, t * anim.spinSpeed, 0.0f };

                anim.obj->setPosition(pos);
                anim.obj->setRotation(rot);
                anim.obj->apply();
            }
        }
    };


    export class AnimatedTestScene : public bm::gfx::Scene
    {
    public:

        static Traits<AnimatedTestScene>::Ptr make()
        {
            return std::make_unique<AnimatedTestScene>();
        }

        AnimatedTestScene()
            : Scene("Animated Test Scene")
        {
            using namespace bm::gfx;

            // === GRID ===
            {
                auto grid = Object::make<Grid>();
                grid->apply();
                addObject(std::move(grid));
            }

            // === SINGLE LIGHT ===
            {
                auto light = Light::make();
                light->setPosition({ 0.0f, 10.0f, 0.0f });
                light->setColor({ 1.0f, 1.0f, 1.0f });
                addLight(std::move(light));
            }

            buildSceneObjects();
        }

        // ------------------------------------------------------------------
        // UPDATE ANIMATIONS
        // ------------------------------------------------------------------
        void onUpdate(float dt) override
        {
            m_time += dt;

            animateLight(dt);
            animateMainTower(dt);
            animateCrystal(dt);
            animatePlatforms(dt);
            animateOrbitalRing(dt);
            animateSpikes(dt);
        }

    private:

        // ---------------------------------------------------------------
        // BUILD SCENE OBJECTS (one-time)
        // ---------------------------------------------------------------
        void buildSceneObjects()
        {
            using namespace bm::gfx;

            // -------------------------------------------------------------
            // Main Tower
            // -------------------------------------------------------------
            {
                auto tower = Object::make<Cube>();
                tower->setPosition({ 0.0f, 6.0f, 0.0f });
                tower->setScale({ 1.5f, 12.0f, 1.5f });
                tower->setColor({ 0.1f, 0.1f, 0.15f });
                tower->apply();

                m_mainTower = tower.get();
                addObject(std::move(tower));
            }

            // Antenna
            {
                auto a = Object::make<Cube>();
                a->setPosition({ 0.0f, 12.5f, 0.0f });
                a->setScale({ 0.2f, 1.0f, 0.2f });
                a->setColor({ 1.0f, 0.0f, 0.0f });
                a->apply();
                addObject(std::move(a));
            }

            // -------------------------------------------------------------
            // Crystal (top of pyramid)
            // -------------------------------------------------------------
            {
                auto c = Object::make<Cube>();
                c->setPosition({ -8.0f, 2.0f, -8.0f });
                c->setScale({ 0.3f, 0.8f, 0.3f });
                c->setRotation({ 0.0f, 45.0f, 0.0f });
                c->setColor({ 0.3f, 0.8f, 1.0f });
                c->apply();

                m_crystal = c.get();
                addObject(std::move(c));
            }

            // -------------------------------------------------------------
            // Floating platforms
            // -------------------------------------------------------------
            for (int i = 0; i < 6; ++i)
            {
                float angle = (i / 6.f) * 2.f * 3.14159f;
                float radius = 10.f;

                auto p = Object::make<Square>();
                p->setPosition({ radius * cos(angle), 1.5f, radius * sin(angle) });
                p->setScale({ 1.5f, 0.1f, 1.5f });
                p->setRotation({ 0.0f, angle * 57.3f, 0.0f });
                p->setColor({ 0.4f, 0.3f, 0.6f });
                p->apply();

                m_platforms.push_back(p.get());
                addObject(std::move(p));
            }

            // -------------------------------------------------------------
            // Orbital ring segments
            // -------------------------------------------------------------
            const int ringSegments = 20;
            const float ringRadius = 15.f;

            for (int i = 0; i < ringSegments; ++i)
            {
                float angle = (i / float(ringSegments)) * 2.f * 3.14159f;

                auto seg = Object::make<Cube>();
                seg->setPosition({ ringRadius * cos(angle), 8.0f, ringRadius * sin(angle) });
                seg->setScale({ 0.3f, 0.3f, 1.2f });
                seg->setRotation({ 0.0f, angle * 57.3f + 90.f, 0.0f });

                // rainbow color ring
                float t = i / float(ringSegments);
                seg->setColor({
                    0.5f + 0.5f * sin(t * 6.2831f),
                    0.5f + 0.5f * sin(t * 6.2831f + 2.09f),
                    0.5f + 0.5f * sin(t * 6.2831f + 4.18f)
                    });

                seg->apply();
                m_ring.push_back(seg.get());
                addObject(std::move(seg));
            }

            // -------------------------------------------------------------
            // Decorative spikes around tower
            // -------------------------------------------------------------
            for (int i = 0; i < 4; ++i)
            {
                float angle = (i / 4.f) * 2.f * 3.14159f;

                auto s = Object::make<Triangle>();
                s->setPosition({ 2.5f * cos(angle), 0.5f, 2.5f * sin(angle) });
                s->setScale({ 0.8f, 2.0f, 0.8f });
                s->setRotation({ 0.0f, angle * 57.3f, 0.0f });
                s->setColor({ 1.0f, 0.3f, 0.2f });
                s->apply();

                m_spikes.push_back(s.get());
                addObject(std::move(s));
            }
        }

        // ---------------------------------------------------------------
        // ANIMATIONS
        // ---------------------------------------------------------------
        void animateLight(float dt)
        {
            using namespace bm::gfx;

            auto& lights = getLights();
            if (lights.empty()) return;

            float r = 20.0f;
            float x = r * cos(m_time * 0.5f);
            float z = r * sin(m_time * 0.5f);

            lights[0]->setPosition({ x, 10.f, z });
        }

        void animateMainTower(float dt)
        {
            if (!m_mainTower) return;

            float yaw = m_time * 5.0f; // slow rotation
            m_mainTower->setRotation({ 0.0f, yaw, 0.0f });
            m_mainTower->apply();
        }

        void animateCrystal(float dt)
        {
            if (!m_crystal) return;

            float y = 2.0f + sin(m_time * 2.0f) * 0.2f;

            m_crystal->setPosition({ -8.0f, y, -8.0f });
            m_crystal->setRotation({ 0.0f, m_time * 90.f, 0.0f });
            m_crystal->apply();
        }

        void animatePlatforms(float dt)
        {
            int i = 0;
            for (auto* p : m_platforms)
            {
                float phase = i * 0.8f;
                float y = 1.5f + sin(m_time * 1.5f + phase) * 0.4f;

                glm::vec3 pos = p->getPosition();
                pos.y = y;
                p->setPosition(pos);
                p->apply();

                ++i;
            }
        }

        void animateOrbitalRing(float dt)
        {
            for (auto* seg : m_ring)
            {
                glm::vec3 r = seg->getRotation();
                r.y += dt * 40.0f;
                seg->setRotation(r);
                seg->apply();
            }
        }

        void animateSpikes(float dt)
        {
            float pul = 1.0f + sin(m_time * 4.0f) * 0.1f;

            for (auto* s : m_spikes)
            {
                glm::vec3 sc = { 0.8f * pul, 2.0f * pul, 0.8f * pul };
                s->setScale(sc);
                s->apply();
            }
        }

    private:

        float m_time = 0.f;

        bm::gfx::Object* m_mainTower = nullptr;
        bm::gfx::Object* m_crystal = nullptr;

        std::vector<bm::gfx::Object*> m_platforms;
        std::vector<bm::gfx::Object*> m_ring;
        std::vector<bm::gfx::Object*> m_spikes;
    };


}