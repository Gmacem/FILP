#pragma once

#include <string>
#include <iostream>

struct Block {
    int strong_count;
    int weak_count;
};

class SharedPtr;

class WeakPtr {
    friend SharedPtr;

private:
    std::string* pointer_;
    Block* counter_;

public:
    WeakPtr();
    WeakPtr(const SharedPtr& ptr);

    WeakPtr(const WeakPtr& rhs);
    WeakPtr(WeakPtr&& rhs);
    WeakPtr& operator=(const WeakPtr& other);
    WeakPtr& operator=(WeakPtr&& other);
    ~WeakPtr();

    bool IsExpired() const;

    SharedPtr Lock();
};

class SharedPtr {
    friend WeakPtr;

public:
    SharedPtr() {
        counter_ = new Block{1, 0};
    }

    SharedPtr(std::string* pointer) {
        pointer_ = pointer;
        counter_ = new Block{1, 0};
    }

    SharedPtr(const WeakPtr& ptr) {
        pointer_ = ptr.IsExpired() ? nullptr : ptr.pointer_;
        counter_ = ptr.counter_;
        if (counter_ != nullptr) {
            ++(counter_->strong_count);
        }
    }

    SharedPtr(const SharedPtr& rhs) {
        pointer_ = rhs.pointer_;
        counter_ = rhs.counter_;
        if (counter_ != nullptr) {
            ++(counter_->strong_count);
        }
    }

    SharedPtr(SharedPtr&& rhs) {
        pointer_ = rhs.pointer_;
        counter_ = rhs.counter_;
        rhs.pointer_ = nullptr;
        rhs.counter_ = nullptr;
    }

    SharedPtr& operator=(const SharedPtr& other) {
        if (&other != this) {
            this->~SharedPtr();
            pointer_ = other.pointer_;
            counter_ = other.counter_;
            if (counter_ != nullptr) {
                ++(counter_->strong_count);
            }
        }
        return *this;
    }

    SharedPtr& operator=(SharedPtr&& other) {
        if (&other != this) {
            this->~SharedPtr();
            pointer_ = other.pointer_;
            counter_ = other.counter_;
            other.pointer_ = nullptr;
            other.counter_ = nullptr;
        }
        return *this;
    }

    std::string operator*() {
        if (pointer_ == nullptr) {
            return "";
        }
        return *pointer_;
    }

    void Swap(SharedPtr& rhs) {
        std::swap(pointer_, rhs.pointer_);
        std::swap(counter_, rhs.counter_);
    }

    void Reset(std::string* pointer) {
        this->~SharedPtr();
        pointer_ = pointer;
        counter_ = new Block{1, 0};
    }

    std::string* Get() {
        if (counter_ == nullptr || counter_->strong_count == 0) {
            return nullptr;
        }
        return pointer_;
    }

    ~SharedPtr() {
        if (counter_ == nullptr) {
            return;
        }
        --(counter_->strong_count);
        if (counter_->strong_count == 0) {
            delete pointer_;
        }
        if (counter_->strong_count + counter_->weak_count == 0) {
            delete counter_;
        }
    }

private:
    std::string* pointer_ = nullptr;
    Block* counter_;
};

WeakPtr::WeakPtr() {
    pointer_ = nullptr;
    counter_ = nullptr;
}

WeakPtr::WeakPtr(const SharedPtr& ptr) {
    pointer_ = ptr.pointer_;
    counter_ = ptr.counter_;
    if (counter_ != nullptr) {
        ++counter_->weak_count;
    }
}

WeakPtr::WeakPtr(const WeakPtr& rhs) {
    pointer_ = rhs.pointer_;
    counter_ = rhs.counter_;
    if (counter_ != nullptr) {
        ++counter_->weak_count;
    }
}
WeakPtr::WeakPtr(WeakPtr&& rhs) {
    pointer_ = rhs.pointer_;
    counter_ = rhs.counter_;
    rhs.pointer_ = nullptr;
    rhs.counter_ = nullptr;
}

WeakPtr& WeakPtr::operator=(const WeakPtr& other) {
    if (&other != this) {
        this->~WeakPtr();
        pointer_ = other.pointer_;
        counter_ = other.counter_;
        if (counter_ != nullptr) {
            ++(counter_->weak_count);
        }
    }
    return *this;
}

WeakPtr& WeakPtr::operator=(WeakPtr&& other) {
    if (&other != this) {
        this->~WeakPtr();
        pointer_ = other.pointer_;
        counter_ = other.counter_;
        other.pointer_ = nullptr;
        other.counter_ = nullptr;
    }
    return *this;
}

WeakPtr::~WeakPtr() {
    if (counter_ == nullptr) {
        return;
    }
    --(counter_->weak_count);
    if (counter_->strong_count + counter_->weak_count == 0) {
        delete counter_;
    }
}

bool WeakPtr::IsExpired() const {
    return counter_ == nullptr || counter_->strong_count == 0;
}

SharedPtr WeakPtr::Lock() {
    if (IsExpired()) {
        return SharedPtr();
    } else {
        return SharedPtr(*this);
    }
}
