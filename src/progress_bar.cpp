#include <progress_bar.hpp>

ProgressBar::ProgressBar(size_t len): _len(len){ }

void ProgressBar::write(){

}

void ProgressBar::bind(Window& window, size_t x, size_t y){
    window.bind_buffer(this, x, y);
}

void ProgressBar::unbind(Window& window){
    window.unbind_buffer(this);
}

#include <cmath>

void ProgressBar::set_per(float per){
    let ends = ((int)self.opts & (int)Opts::Ends) != 0;
    let len = this->_len - 2 * ends; 
    mut max = size_t(floor(per * len));
    if(max > this->_len)
        max = this->_len;
    let min = ends ? 1 : 0;
    if(ends){
        self._buffer.get(0, 0).value()->c = '[';
        self._buffer.get(self._len - 1, 0).value()->c = ']';
    }
    for(size_t i = min; i < max; ++i){
        this->_buffer.get(i, 0).value()->c = self.var_char;
    }
}

Buffer& ProgressBar::get_buffer(){
    return this->_buffer;
}
