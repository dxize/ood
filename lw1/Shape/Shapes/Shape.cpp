#include "Shape.h"
#include <sstream>
#include <iomanip>

//обернуть в неймспейс чтобы не повторять shapes:: везде
shapes::Shape::Shape(std::unique_ptr<IShapeStrategy> strategy, const std::string& color, const std::string& id)
    : m_strategy(std::move(strategy)), m_color(color), m_id(id) {}

void  shapes::Shape::Draw(ICanvas& canvas) const 
{
    m_strategy->Draw(canvas, m_color);
}

void shapes::Shape::Move(double dx, double dy) 
{
    m_strategy->Move(dx, dy);
}

void  shapes::Shape::SetColor(const std::string& color) 
{
    m_color = color;
}

void  shapes::Shape::SetStrategy(std::unique_ptr<IShapeStrategy> strategy) 
{
    m_strategy = std::move(strategy);
}

std::string  shapes::Shape::GetType() const 
{
    return m_strategy->GetType();
}

std::vector<double>  shapes::Shape::GetParameters() const 
{
    return m_strategy->GetParameters();
}

std::string  shapes::Shape::ToString() const 
{
    std::stringstream ss;
    ss << GetType() << " " << m_id << " " << m_color;

    auto params = GetParameters();
    for (double param : params) 
    {
        ss << " " << param;
    }

    std::string extra = m_strategy->GetExtraInfo();
    if (!extra.empty()) 
    {
        ss << " " << extra;
    }

    return ss.str();
}
