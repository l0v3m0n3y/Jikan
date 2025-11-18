# Jikan
api for jikan.moe funny anime site
# main
```cpp
#include "Jikan.h"
#include <iostream>

int main() {
   Jikan api;

    auto anime = api.getAnimeSearch().then([](json::value result) {
        std::cout << "Search results: " << result.serialize() << std::endl;
    });
    anime.wait();
    
    return 0;
}
```

# Launch (your script)
```
g++ -std=c++11 -o main main.cpp -lcpprest -lssl -lcrypto -lpthread -lboost_system -lboost_chrono -lboost_thread
./main
```

