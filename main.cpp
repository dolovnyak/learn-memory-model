#include <atomic>
#include <cassert>
#include <thread>
#include <barrier>

std::atomic<int> a{0};
std::atomic<int> b{0};
std::barrier barrier{2};
static const size_t iterations = 100000000;

/// for arm architecture
void f1() {
    for (size_t i = 0; i < iterations; ++i) {
//        /// 1: operations order is mixed
//        b.store(1, std::memory_order_relaxed);
//        a.store(1, std::memory_order_relaxed);
//
//        /// 2: operations order is consecutive
//        b.store(1, std::memory_order_seq_cst);
//        a.store(1, std::memory_order_seq_cst);
//
//        /// 3: any operations before a.store will process before it, so order is correct (store b then a)
//        b.store(1, std::memory_order_relaxed);
//        a.store(1, std::memory_order_release);
//
//        /// 4: any operations before a.store will process before it, so order is correct (store b then a)
//        b.store(1, std::memory_order_relaxed);
//        a.store(1, std::memory_order_release);
//
//        /// 5: any operations before b.store will process before it, and order isn't correct because a goes after b
//        b.store(1, std::memory_order_release);
//        a.store(1, std::memory_order_relaxed);

        /// 6: operations order is mixed
        b.store(1, std::memory_order_relaxed);
        a.store(1, std::memory_order_relaxed);

        barrier.arrive_and_wait();
    }
}

void f2() {
    for (size_t i = 0; i < iterations; ++i) {
//        /// 1: could fail, operations will process in mixed order
//        while (!a.load(std::memory_order_relaxed)) {}
//        assert(b.load(std::memory_order_relaxed) == 1);
//
//        /// 2: shouldn't fail, operations will process consequentially
//        while (a.load(std::memory_order_seq_cst) != 1) {}
//        assert(b.load(std::memory_order_seq_cst) == 1);
//
//        /// 3: shouldn't fail, any operations after "a.load" will process after it, so load order correct "load a then b"
//        while (!a.load(std::memory_order_acquire)) {}
//        assert(b.load(std::memory_order_relaxed) == 1);
//
//        /// 4: could fail, despite store order is correct. Any operations after "b.load" will process after it, but "a.load" is in front of it. So it's possible to have "load b, then a"
//        while (!a.load(std::memory_order_relaxed)) {}
//        assert(b.load(std::memory_order_acquire) == 1);
//
//        /// 5: could fail, despite load order is correct, because store order isn't correct. It's strange but it isn't fail.
//        while (!a.load(std::memory_order_acquire)) {}
//        assert(b.load(std::memory_order_relaxed) == 1);

        /// 6: could fail, despite load order is correct, because store order isn't correct. It's strange but it isn't fail too even with mixed order store.
        while (!a.load(std::memory_order_acquire)) {}
        assert(b.load(std::memory_order_relaxed) == 1);


        b.store(0, std::memory_order_seq_cst);
        a.store(0, std::memory_order_seq_cst);

        barrier.arrive_and_wait();
    }
}

int main() {
    a.store(0);
    b.store(0);
    std::thread writer(f1);
    std::thread loader(f2);
    writer.join();
    loader.join();
}
