#pragma once
#include "ICommand.h"
#include "../CPicture.h"

class CAddShapeCommand : public ICommand 
{
public:
	CAddShapeCommand(CPicture& picture, const std::string id, std::unique_ptr<IShape> shape);
	void Execute() override;
private:
	CPicture& m_picture;
	const std::string m_id;
	std::unique_ptr<IShape> m_shape;
};

class CChangeColorCommand : public ICommand
{
public:
	CChangeColorCommand(CPicture& picture, const std::string id, uint32_t newColor);
	void Execute() override;
private:
	CPicture& m_picture;
	const std::string m_id;
	uint32_t m_color;
};

class CChangeShapeCommand : public ICommand
{
public:
	CChangeShapeCommand(CPicture& picture, const std::string id, std::unique_ptr<IShape> newShape);
	void Execute() override;
private:
	CPicture& m_picture;
	const std::string m_id;
	std::unique_ptr<IShape> m_shape;
};

class CDeleteShapeCommand : public ICommand
{
public:
	CDeleteShapeCommand(CPicture& picture, const std::string id);
	void Execute() override;
private:
	CPicture& m_picture;
	const std::string m_id;
};

class CDrawPictureCommand : public ICommand //допил нужен!!!!!!
{
public:
	void Execute() override;
private:

};

class CDrawShapeCommand : public ICommand //допил нужен!!!!!!!!!!
{
public:
	void Execute() override;
};

class CListCommand : public ICommand
{
public:
	CListCommand(CPicture& picture);
	void Execute() override;
private:
	CPicture& m_picture;
};

class CMovePictureCommand : public ICommand
{
public:
	CMovePictureCommand(CPicture& picture, double dx, double dy);
	void Execute() override;
private:
	CPicture& m_picture;
	double m_dx, m_dy;
};

class CMoveShapeCommand : public ICommand
{
public:
	CMoveShapeCommand(CPicture& picture, std::string id, double dx, double dy);
	void Execute() override;
private:
	CPicture& m_picture;
	std::string m_id;
	double m_dx, m_dy;
};