module;

#include <stdlib.h>

export module scenes;

import std;

import BridgeMaker;

using namespace bm;
using namespace bm::gfx;

namespace scene
{


    export class VRMLMaterialsScene : public bm::gfx::Scene
    {
    public:

        static Traits<VRMLMaterialsScene>::Ptr make()
        {
            return std::make_unique<VRMLMaterialsScene>();
        }

        VRMLMaterialsScene()
            : Scene("VRML Materials Demo Scene")
        {
            using namespace bm::gfx;

            // -----------------------------------------
            // CREATE ANIMATED LIGHT
            // -----------------------------------------
            auto light = Light::make();
            light->setColor({ 1.f, 1.f, 1.f });
            light->setPosition({ 5.f, 5.f, 0.f });
            addLight(std::move(light));

            // -----------------------------------------
            // GROUND PLANE
            // -----------------------------------------
            {
                auto ground = Object::make<Square>();
                ground->setPosition({ 0.f, 0.f, 0.f });
                ground->setScale({ 50.f, 1.f, 50.f });
                ground->setColor({ 0.2f, 0.2f, 0.25f });
                ground->apply();
                addObject(std::move(ground));
            }

            // -----------------------------------------
            // MATERIAL SETUP FROM CHEATSHEET
            // -----------------------------------------

            struct MDef
            {
                const char* name;
                glm::vec3 amb;
                glm::vec3 dif;
                glm::vec3 spec;
                float shininess;  // cheat sheet shininess * 128
            };

            std::vector<MDef> materials =
            {
                // name        amb                              dif                               spec                           shininess
                {"emerald",     {0.0215f,0.1745f,0.0215f},       {0.07568f,0.61424f,0.07568f},     {0.633f,0.727811f,0.633f},     0.6f * 128.0f},
                {"jade",        {0.135f,0.2225f,0.1575f},        {0.54f,0.89f,0.63f},               {0.316228f,0.316228f,0.316228f}, 0.1f * 128.0f},
                {"obsidian",    {0.05375f,0.05f,0.06625f},       {0.18275f,0.17f,0.22525f},         {0.332741f,0.328634f,0.346435f}, 0.3f * 128.0f},
                {"pearl",       {0.25f,0.20725f,0.20725f},       {1.0f,0.829f,0.829f},              {0.296648f,0.296648f,0.296648f}, 0.088f * 128.0f},
                {"ruby",        {0.1745f,0.01175f,0.01175f},     {0.61424f,0.04136f,0.04136f},      {0.727811f,0.626959f,0.626959f}, 0.6f * 128.0f},
                {"turquoise",   {0.1f,0.18725f,0.1745f},         {0.396f,0.74151f,0.69102f},         {0.297254f,0.30829f,0.306678f}, 0.1f * 128.0f},
                {"brass",       {0.329412f,0.223529f,0.027451f}, {0.780392f,0.568627f,0.113725f},   {0.992157f,0.941176f,0.807843f}, 0.21794872f * 128.0f},
                {"bronze",      {0.2125f,0.1275f,0.054f},        {0.714f,0.4284f,0.18144f},         {0.393548f,0.271906f,0.166721f}, 0.2f * 128.0f},
                {"chrome",      {0.25f,0.25f,0.25f},             {0.4f,0.4f,0.4f},                   {0.774597f,0.774597f,0.774597f}, 0.6f * 128.0f},
                {"copper",      {0.19125f,0.0735f,0.0225f},      {0.7038f,0.27048f,0.0828f},         {0.256777f,0.137622f,0.086014f}, 0.1f * 128.0f},
                {"gold",        {0.24725f,0.1995f,0.0745f},      {0.75164f,0.60648f,0.22648f},       {0.628281f,0.555802f,0.366065f}, 0.4f * 128.0f},
                {"silver",      {0.19225f,0.19225f,0.19225f},    {0.50754f,0.50754f,0.50754f},       {0.508273f,0.508273f,0.508273f}, 0.4f * 128.0f},

                {"black plastic",   {0.f,0.f,0.f},               {0.01f,0.01f,0.01f},                {0.50f,0.50f,0.50f},            0.25f * 128.0f},
                {"cyan plastic",    {0.f,0.f,0.f},               {0.f,0.50980392f,0.50980392f},      {0.50196078f,0.50196078f,0.50196078f}, 0.25f * 128.0f},
                {"green plastic",   {0.f,0.f,0.f},               {0.1f,0.35f,0.1f},                  {0.45f,0.55f,0.45f},            0.25f * 128.0f},
                {"red plastic",     {0.f,0.f,0.f},               {0.5f,0.f,0.f},                     {0.7f,0.6f,0.6f},                0.25f * 128.0f},
                {"white plastic",   {0.f,0.f,0.f},               {0.55f,0.55f,0.55f},                {0.70f,0.70f,0.70f},            0.25f * 128.0f},
                {"yellow plastic",  {0.f,0.f,0.f},               {0.5f,0.5f,0.0f},                   {0.60f,0.60f,0.50f},            0.25f * 128.0f},

                {"black rubber",    {0.02f,0.02f,0.02f},         {0.01f,0.01f,0.01f},                {0.4f,0.4f,0.4f},               0.078125f * 128.0f},
                {"cyan rubber",     {0.f,0.f,0.f},               {0.f,0.05f,0.05f},                  {0.04f,0.7f,0.7f},              0.078125f * 128.0f},
                {"green rubber",    {0.f,0.f,0.f},               {0.f,0.05f,0.f},                    {0.04f,0.7f,0.04f},             0.078125f * 128.0f},
                {"red rubber",      {0.f,0.f,0.f},               {0.05f,0.f,0.f},                    {0.7f,0.04f,0.04f},             0.078125f * 128.0f},
                {"white rubber",    {0.05f,0.05f,0.05f},         {0.5f,0.5f,0.5f},                   {0.7f,0.7f,0.7f},               0.078125f * 128.0f},
                {"yellow rubber",   {0.05f,0.05f,0.f},           {0.5f,0.5f,0.4f},                   {0.7f,0.7f,0.04f},              0.078125f * 128.0f},
            };

            // -----------------------------------------
            // CREATE SPHERES FOR EACH MATERIAL
            // -----------------------------------------

            const int perRow = 7;
            const float spacing = 3.5f;

            int index = 0;

            for (auto& m : materials)
            {
                float x = (index % perRow) * spacing - (perRow * spacing * 0.5f);
                float z = (index / perRow) * spacing - 10.f;

                auto sphere = Object::make<Sphere>();
                sphere->setPosition({ x, 1.2f, z });
                sphere->setScale({ 1.0f, 1.0f, 1.0f });

                auto mat = sphere->getMaterial();
                mat->setColor({ 1,1,1,1 });
                mat->setAmbient(m.amb);
                mat->setDiffuse(m.dif);
                mat->setSpecular(m.spec);
                mat->setShininess(m.shininess);

                sphere->apply();
                addObject(std::move(sphere));

                index++;
            }

            // -----------------------------------------
            // WHITE BULB TO SHOW LIGHT POSITION
            // -----------------------------------------
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
        // ANIMATE LIGHT
        // -----------------------------------------------------------
        void onUpdate(float dt) override
        {
            m_time += dt;

            float r = 12.f;
            float x = r * std::cos(m_time * 0.6f);
            float z = r * std::sin(m_time * 0.6f);
            float y = 4.f + std::sin(m_time * 0.8f) * 1.5f;

            glm::vec3 pos = { x, y, z };

            auto& lights = getLights();
            if (!lights.empty())
                lights[0]->setPosition(pos);

            if (m_lightSphere)
            {
                m_lightSphere->setPosition(pos);
                m_lightSphere->apply();
            }
        }

    private:

        float m_time = 0.f;
        bm::gfx::Object* m_lightSphere = nullptr;
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