#include "examples/service.h"

class Automobile {
public:
    void Start() {
    }

    void SpeedUp() {
    }

    void SlowDown() {
    }

    void Stop() {
    }
};

class Driver {
private:
    Automobile Automobile_;

public:
    void Drive() {
        Automobile_.Start();
        Automobile_.SpeedUp();
        Automobile_.SlowDown();
        Automobile_.Stop();
    }
};

int main() {
    DoJob();

    return 0;
}
