#pragma once

#include "IBeverage.h"

// Базовая реализация напитка, предоставляющая его описание
class CBeverage : public IBeverage
{
public:
	CBeverage(const std::string & description)
		:m_description(description)
	{}

	std::string GetDescription()const override final
	{
		return m_description;
	}
private:
	std::string m_description;
};

// Кофе
class CCoffee : public CBeverage
{
public:
	CCoffee(const std::string& description = "Coffee")
		:CBeverage(description) 
	{}

	double GetCost() const override 
	{
		return 60; 
	}
};

//Капучино
enum class CappuccinoSize 
{ 
	Standard, Double 
};

class CCappuccino : public CCoffee
{
public:
	CCappuccino(CappuccinoSize size = CappuccinoSize::Standard)
		: CCoffee(size == CappuccinoSize::Standard ? "Cappuccino" : "Double cappuccino")
		, m_size(size)
	{}

	double GetCost() const override
	{
		return (m_size == CappuccinoSize::Standard) ? 80 : 120;
	}

private:
	CappuccinoSize m_size;
};

//Латте
enum class LatteSize 
{ 
	Standard, Double 
};

class CLatte : public CCoffee
{
public:
	CLatte(LatteSize size = LatteSize::Standard)
		: CCoffee(size == LatteSize::Standard ? "Latte" : "Double latte")
		, m_size(size)
	{}

	double GetCost() const override
	{
		return (m_size == LatteSize::Standard) ? 90 : 130;
	}	

private:
	LatteSize m_size;
};

enum class TeaType
{
	BlackTea,
	GreenTea,
	WhiteTea,
	YellowTea
};

// Чай
class CTea : public CBeverage
{
public:
	CTea(TeaType type = TeaType::BlackTea)
		:CBeverage("Tea ( " + GetTypeOfTea(type) + ")")
	{}

	double GetCost() const override 
	{
		return 30; 
	}

private:
	std::string GetTypeOfTea(TeaType type)
	{
		switch (type)
		{
		case TeaType::BlackTea:
			return "Black";
		case TeaType::GreenTea:
			return "Green";
		case TeaType::WhiteTea:
			return "White";
		case TeaType::YellowTea:
			return "Yellow";
		}
		return "Unknown";
	}
};

//Молочный коктейль
enum class MilkshakeSize 
{
	Small,
	Medium, 
	Large 
};

class CMilkshake : public CBeverage
{
public:
	CMilkshake(MilkshakeSize size = MilkshakeSize::Medium)
		: CBeverage(GetMilkshakeDescription(size))
		, m_size(size)
	{}

	double GetCost() const override
	{
		switch (m_size)
		{
		case MilkshakeSize::Small: 
			return 50;
		case MilkshakeSize::Medium:
			return 60;
		case MilkshakeSize::Large: 
			return 80;
		}
		return 0;
	}

private:
	MilkshakeSize m_size;

	std::string GetMilkshakeDescription(MilkshakeSize size)
	{
		switch (size)
		{
		case MilkshakeSize::Small: 
			return "Small milkshake";
		case MilkshakeSize::Medium: 
			return "Medium milkshake";
		case MilkshakeSize::Large: 
			return "Large milkshake";
		}
		return "Milkshake";
	}
};