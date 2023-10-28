#ifndef __SYSTEM_RENDER_HPP__
#define __SYSTEM_RENDER_HPP__

#include "progress_bar.hpp"
#include "renderer.hpp"
#include "system.hpp"

class MemoryBar : public ProgressBar{
private:
    ProgressBar _used;
    ProgressBar _buffers;
    ProgressBar _shared;
    ProgressBar _chached;
public:
    MemoryBar(size_t len);

    void write() override;
};

#endif
