#pragma once
#include "IShapeStrategy.h"
#include <memory>
#include <string>
#include <vector>
#include <functional>
#include <unordered_map>

class ShapeFactory 
{
public:
    ShapeFactory();
    std::unique_ptr<IShapeStrategy> CreateStrategy(const std::string& type,
        const std::vector<std::string>& params);
private:
    std::unordered_map<std::string,
        std::function<std::unique_ptr<IShapeStrategy>(const std::vector<std::string>&)>>
        m_creators;
};
