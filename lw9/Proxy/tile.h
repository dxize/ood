#pragma once
#include <array>
#include <cassert>

#include "geom.h"

class Tile
{
public:
	// Размер тайла 8*8 пикселей.
	constexpr static int SIZE = 8;

	// Конструктор по умолчанию. Заполняет тайл указанным цветом.
	Tile(char color = ' ') noexcept
	{
		// Заполняем массив пикселей указанным цветом.
		for (auto& row : m_data)
		{
			row.fill(color);
		}

		// -------------- не удалять ------------
		assert(m_instanceCount >= 0);
		++m_instanceCount; // Увеличиваем счётчик тайлов (для целей тестирования).
		// -------------- не удалять ------------
	}

	Tile(const Tile& other)
		: m_data(other.m_data)
	{
		// -------------- не удалять ------------
		assert(m_instanceCount >= 0);
		++m_instanceCount; // Увеличиваем счётчик тайлов (для целей тестирования).
		// -------------- не удалять ------------
	}

	~Tile()
	{
		// -------------- не удалять ------------
		--m_instanceCount; // Уменьшаем счётчик тайлов.
		assert(m_instanceCount >= 0);
		// -------------- не удалять ------------
	}

	/**
	 * Изменяет цвет пикселя тайла.
	 * Если координаты выходят за пределы тайла, метод ничего не делает.
	 */
	void SetPixel(Point p, char color) noexcept
	{
		// Проверка границ и запись в массив пикселей.
		if (p.x < 0 || p.y < 0 || p.x >= SIZE || p.y >= SIZE)
		{
			return;
		}

		m_data[p.y][p.x] = color;
	}

	/**
	 * Возвращает цвет пикселя. Если координаты выходят за пределы тайла, возвращается пробел.
	 */
	char GetPixel(Point p) const noexcept
	{
		// Проверка границ и чтение из массива пикселей.
		if (p.x < 0 || p.y < 0 || p.x >= SIZE || p.y >= SIZE)
		{
			return ' ';
		}

		return m_data[p.y][p.x];
	}

	// Возвращает количество экземпляра класса Tile в программе.
	static int GetInstanceCount() noexcept
	{
		// -------------- не удалять ------------
		return m_instanceCount;
		// -------------- не удалять ------------
	}

private:
	// -------------- не удалять ------------
	inline static int m_instanceCount = 0;
	// -------------- не удалять ------------

	/* Разместите здесь поля для хранения пикселей тайла. */
	std::array<std::array<char, SIZE>, SIZE> m_data{};
};
