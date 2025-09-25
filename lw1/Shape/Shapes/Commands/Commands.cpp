#include "Commands.h"
#include "iostream"

CAddShapeCommand::CAddShapeCommand(CPicture& picture, const std::string id,
	std::unique_ptr<IShape> shape)
	: m_picture(picture)
	, m_id(id)
	, m_shape(std::move(shape))
{};

void CAddShapeCommand::Execute()
{
	m_picture.AddShape(m_id, std::move(m_shape));
}

CChangeColorCommand::CChangeColorCommand(CPicture& picture, const std::string id,
	uint32_t newColor) 
	: m_picture(picture)
	, m_id(id)
	, m_color(newColor)
{};

void CChangeColorCommand::Execute() 
{
	m_picture.ChangeShapeColor(m_id, m_color);
}

CChangeShapeCommand::CChangeShapeCommand(CPicture& picture, const std::string id,
	std::unique_ptr<IShape> newShape)
	: m_picture(picture)
	, m_id(id)
	, m_shape(std::move(newShape))
{};

void CChangeShapeCommand::Execute()  
{
	m_picture.ChangeShape(m_id, std::move(m_shape));
}

CDeleteShapeCommand::CDeleteShapeCommand(CPicture& picture, const std::string id)
	: m_picture(picture)
	, m_id(id)
{};

void CDeleteShapeCommand::Execute()  
{
	m_picture.DeleteShape(m_id);
}
void CDrawPictureCommand::Execute()  
{
	//лютый допил
}
void CDrawShapeCommand::Execute()  
{
	//лютый допил
}

CListCommand::CListCommand(CPicture& picture)
	: m_picture(picture)
{};

void CListCommand::Execute()  
{
	m_picture.ListShapes();
}

CMovePictureCommand::CMovePictureCommand(CPicture& picture, double dx,
	double dy)
	: m_picture(picture)
	, m_dx(dx)
	, m_dy(dy)
{};

void CMovePictureCommand::Execute()  
{
	m_picture.MoveAllShapes(m_dx, m_dy);
}

CMoveShapeCommand::CMoveShapeCommand(CPicture& picture, std::string id,
	double dx, double dy)
	: m_picture(picture)
	, m_id(id)
	, m_dx(dx)
	, m_dy(dy)
{};

void CMoveShapeCommand::Execute()  
{
	m_picture.MoveShape(m_id, m_dx, m_dy);
}