#pragma once

#include <vector>

template <typename TData>
class Repository {
private:
    std::vector<TData> Data_;

public:
    TData Get(int id) const {
        return Data_[id];
    }

    void Update(TData update) requires requires { update.Id; } {
        Data_[update.Id] = std::move(update);
    }

    int Add(TData data) requires requires { data.Id; } {
        int id = Data_.size();

        data.Id = id;
        Data_.emplace_back(std::move(data));

        return id;
    }

    void Remove(int id) {
        Data_.erase(std::next(Data_.begin(), id));
    }
};