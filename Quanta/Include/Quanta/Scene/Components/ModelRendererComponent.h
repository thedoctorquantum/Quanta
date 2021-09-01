#pragma once

#include "../../Renderer/Model.h"
#include "../../Renderer/Material.h"

namespace Quanta
{
    class ModelRendererComponent final 
    {
    public:
        std::shared_ptr<Model> model;
    };
}