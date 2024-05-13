#pragma once

namespace {
    class Test {
        private:
        public:
            Test();
            virtual ~Test();

            virtual void OnUpdate(float deltaTime) {}
            virtual void OnRender() {}
            virtual void OnImGuiRender() {}
    };
}