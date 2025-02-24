#ifndef ITEM_GENERATOR_HPP
#define ITEM_GENERATOR_HPP

#include "logger.hpp"
#include "event_dispatch.hpp"
#include "item_reader.hpp"
#include "interval_thread.hpp"
#include "double_buffer.hpp"
#include "process.hpp"
#include <mutex>
#include <string>

class ProcessItemReader : public EventListener {
public:
    ProcessItemReader(std::string command, std::string startQuery);
    ~ProcessItemReader();
    void onStart() override;
    void onLoop() override;
    void preOnEvent(EventType eventType) override;
    void onEvent(std::shared_ptr<Event> event) override;

    int copyItems(Item *buffer, int idx, int n);
    int size();

private:
    std::mutex m_mut;
    std::string m_newQuery;
    std::string m_query;

    Process m_process;
    IntervalThread m_interval;

    EventDispatch &m_dispatch = EventDispatch::instance();
    Logger m_logger = Logger("ProcessItemReader");
    DoubleBuffer<std::vector<Item>> m_items;
    ItemReader m_itemReader;

    int m_readMax;
    bool m_queryChanged = false;

    bool m_intervalPassed = false;
    bool m_intervalActive = false;

    void dispatchItems();
    void dispatchAllRead(bool value);

    bool readFirstBatch();
    bool readItem();

    void startChildProcess();

    void freeItems(std::vector<Item> &items);
};

#endif
