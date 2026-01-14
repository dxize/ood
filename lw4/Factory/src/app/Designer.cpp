#include "../../include/app/Designer.h"

#include "../../include/domain/PictureDraft.h"
#include "../../include/factory/IShapeFactory.h"
#include "../../include/util/StringUtil.h"

#include <string>

namespace app
{
    Designer::Designer(const factory::IShapeFactory& factory)
        : m_factory(factory)
    {
    }

    domain::PictureDraft Designer::CreateDraft(std::istream& input) const
    {
        domain::PictureDraft draft;

        std::string line;
        std::size_t lineNo = 0;

        while (std::getline(input, line))
        {
            ++lineNo;

            if (util::IsBlankOrComment(line))
            {
                continue;
            }

            try
            {
                auto shape = m_factory.CreateShape(util::Trim(line));
                draft.AddShape(std::move(shape));
            }
            catch (const std::exception& e)
            {
                throw std::runtime_error("Line " + std::to_string(lineNo) + ": " + e.what());
            }
        }

        return draft;
    }
} 
