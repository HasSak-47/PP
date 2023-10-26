#include <curses.h>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <memory>
#include <optional>
#include <renderer.hpp>

Buffer::Buffer(){}

Buffer::Buffer(size_t width, size_t height):
    data(std::make_unique<Unit[]>(width * height)),
    _width(width),
    _height(height)
{}

Vector2<size_t> Buffer::size(){
    return Vector2{this->_width, this->_height};
}

size_t Buffer::len(){
    return this->_width * this->_height;
}

std::optional<Unit*> Buffer::get(size_t x, size_t y){
    size_t w = this->size().x;
    if(this->len() <= x + (y * w)){
        return {};
    }
    return &this->data[x + (y * w)];
}

Window::Window(){
    initscr();
    noecho();
    cbreak();
    start_color();
    curs_set(0);

    size_t x {getmaxx(stdscr)};
    size_t y {getmaxy(stdscr)};

    this->_present_buffer = Buffer(x, y);
}


Window::~Window(){
    endwin();
}

void Window::bind_buffer(Renderee* parent, size_t x, size_t y){
    this->_renderees.push_back(Renderees{parent, {x, y}});
}

void Window::unbind_buffer(Renderee* parent){
    auto buffer = std::find_if(this->_renderees.begin(), this->_renderees.end(),
            [&](Renderees val){ return val.parent == parent; });
    if(buffer == this->_renderees.end()){
        std::cout << "parent not found!\n";
    }
    else{
        this->_renderees.erase(buffer);
    }
}

static void write_to_buffer(size_t x, size_t y, Buffer& dest, Buffer& src){
    auto s_size = src.size();
    auto d_size = dest.size();

    for(size_t i = 0; i < s_size.x; ++i){
        if(i + x >= d_size.x) continue;
        for(size_t j = 0; j < s_size.y; ++j){
            if(j + y >= d_size.y) continue;
            auto dest_ptr = dest.get(i + x, j + y).value();
            auto  src_ptr =  src.get(i, j).value();
            *dest_ptr = *src_ptr;
        }
    }
}

void Window::render(){
    for(auto& renderee : this->_renderees){
        renderee.parent->write();
        write_to_buffer(renderee.pos.x, renderee.pos.y, this->_present_buffer, renderee.parent->get_buffer());
    }

    auto psize = this->_present_buffer.size();
    for(size_t i = 0; i < psize.x; ++i)
        for(size_t j = 0; j < psize.y; ++j){
            if(char c = this->_present_buffer.get(i, j).value()-> c; isprint(c))
                mvaddch(j, i, c);
            break;
        }
    refresh();
}

Vector2<size_t> Window::get_size() {
    return this->_present_buffer.size();
}

