#pragma once
#include <deque>
#include <memory>

class ICommand
{
public:
    virtual void Execute() = 0;
    virtual void Unexecute() = 0;

    // true -> merged into current command
    virtual bool MergeWith(const std::shared_ptr<ICommand>&) 
    { 
        return false; 
    }

    virtual void OnDrop(bool /*isExecuted*/) {}

    virtual ~ICommand() = default;
};

class CCommandHistory
{
public:
    explicit CCommandHistory(size_t limit = 10);

    void AddAndExecute(const std::shared_ptr<ICommand>& cmd);

    bool CanUndo() const;
    void Undo();

    bool CanRedo() const;
    void Redo();

private:
    size_t m_limit = 10;
    std::deque<std::shared_ptr<ICommand>> m_commands;
    size_t m_next = 0; // index of first redo
};
