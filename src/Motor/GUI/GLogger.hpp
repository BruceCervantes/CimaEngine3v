#pragma once
#include "GLogger.hpp"
#include "../Utils/Lista.hpp"
#include <utility>
#include <imgui.h>

#include "GLayer.hpp"

namespace CE
{
    class GLogger: public GLayer
    {
        public:
            enum class Niveles
            {
                LOG,
                LOG_DEBUG,
                LOG_ERROR,
                LOG_SEVERO
            };
        public:
            static GLogger& Get(void);
            ~GLogger() override{};
            void OnInit(const MotorConfig& des) override;
            void OnUpdate(float dt) override;
            void OnRender(void) override;
            void agregarLog(const std::string& log,Niveles n);

        private:
            ImVec4 nivelColores(Niveles n);
            std::string nivelToString(Niveles n);
        private:
            explicit GLogger();
            Lista<std::pair<std::string,Niveles>> queue;
            static inline GLogger *instance=nullptr;
            std::string m_sfps;


    };

}
