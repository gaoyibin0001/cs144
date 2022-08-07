#include "byte_stream.hh"

#include <algorithm>

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity) { cap = capacity; }

size_t ByteStream::write(const string &data) {
    size_t written_num = 0;
    // printf("%d, %d\n", cap, inner_store.size());
    while ((cap - inner_store.size()) > 0 && written_num < data.size()) {
        // while ((cap - inner_store.size()) > 0) {

        inner_store.push_back(data.at(written_num));
        written_num++;
    }
    bytes_written_num += written_num;
    return written_num;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    string peek(inner_store, 0, len);
    return peek;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
    int pop_length = min(len, inner_store.size());
    inner_store.erase(0, pop_length);
    bytes_read_num += pop_length;
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    string read = peek_output(len);
    pop_output(len);

    return read;
}

void ByteStream::end_input() { is_input_end = true; }

bool ByteStream::input_ended() const { return is_input_end; }

size_t ByteStream::buffer_size() const { return inner_store.size(); }

bool ByteStream::buffer_empty() const { return buffer_size() == 0; }

bool ByteStream::eof() const { return is_input_end && buffer_empty(); }

size_t ByteStream::bytes_written() const { return bytes_written_num; }

size_t ByteStream::bytes_read() const { return bytes_read_num; }

size_t ByteStream::remaining_capacity() const { return cap - inner_store.size(); }
