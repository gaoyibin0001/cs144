#include "wrapping_integers.hh"

// Dummy implementation of a 32-bit wrapping integer

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

uint64_t max_32 = uint64_t(1) << 32;
uint64_t max_31 = uint64_t(1) << 31;

using namespace std;

//! Transform an "absolute" 64-bit sequence number (zero-indexed) into a WrappingInt32
//! \param n The input absolute 64-bit sequence number
//! \param isn The initial sequence number
WrappingInt32 wrap(uint64_t n, WrappingInt32 isn) {
    uint32_t num = n % max_32;
    return WrappingInt32{isn + num};
}

//! Transform a WrappingInt32 into an "absolute" 64-bit sequence number (zero-indexed)
//! \param n The relative sequence number
//! \param isn The initial sequence number
//! \param checkpoint A recent absolute 64-bit sequence number
//! \returns the 64-bit sequence number that wraps to `n` and is closest to `checkpoint`
//!
//! \note Each of the two streams of the TCP connection has its own ISN. One stream
//! runs from the local TCPSender to the remote TCPReceiver and has one ISN,
//! and the other stream runs from the remote TCPSender to the local TCPReceiver and
//! has a different ISN.
// 15 3  13  6 16
uint64_t unwrap(WrappingInt32 n, WrappingInt32 isn, uint64_t checkpoint) {
    // right - 2 31 < checkpoint < right + 2 31
    uint64_t quotient = checkpoint / max_32;
    uint64_t reminder = checkpoint % max_32;

    int32_t relative_num = n - isn;  // todo maybe a bug
    uint64_t num = 0;
    if (relative_num < 0) {
        num = uint64_t(max_32 + relative_num);
    } else {
        num = uint64_t(relative_num);
    }

    if (num <= reminder) {
        if ((reminder - num) > max_31) {
            return (quotient + 1) * max_32 + num;
        } else {
            return quotient * max_32 + num;
        }
    } else {
        if (((num - reminder) > max_31) && (quotient > 0)) {
            return (quotient - 1) * max_32 + num;
        } else {
            return quotient * max_32 + num;
        }
    }
}

// int main() {
//     uint64_t aa = unwrap(WrappingInt32(UINT32_MAX), WrappingInt32(0), 0);
//     return 0;
// }
