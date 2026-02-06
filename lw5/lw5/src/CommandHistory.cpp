#include "CommandHistory.h"
#include <stdexcept>

CCommandHistory::CCommandHistory(size_t limit)
    : m_limit(limit)
{
}

void CCommandHistory::AddAndExecute(const std::shared_ptr<ICommand>& cmd)
{
    // чистим ветку redo(отмененные команды)
    while (m_next < m_commands.size())
    {
        auto last = m_commands.back();
        last->OnDrop(false);
        m_commands.pop_back();
    }

    //одниковые команды мерджим
    if (!m_commands.empty())
    {
        auto& last = m_commands.back();
        if (last->MergeWith(cmd))   
        {
            last->Execute();
            return;
        }
    }

    // выполнение 
    cmd->Execute();
    m_commands.push_back(cmd);
    m_next = m_commands.size();

    // выход за лимит
    while (m_commands.size() > m_limit)
    {
        bool frontExecuted = (m_next > 0);
        m_commands.front()->OnDrop(frontExecuted);

        m_commands.pop_front();
        if (m_next > 0)
        {
            --m_next;
        } 
    }
}

bool CCommandHistory::CanUndo() const
{
    return m_next > 0;
}

void CCommandHistory::Undo()
{
    if (!CanUndo())
    {
        throw std::runtime_error("Undo is not available");
    }
    auto cmd = m_commands[m_next - 1];
    cmd->Unexecute();
    --m_next;
}

bool CCommandHistory::CanRedo() const
{
    return m_next < m_commands.size();
}

void CCommandHistory::Redo()
{
    if (!CanRedo())
    {
        throw std::runtime_error("Redo is not available");
    }
    auto cmd = m_commands[m_next];
    cmd->Execute();
    ++m_next;
}
