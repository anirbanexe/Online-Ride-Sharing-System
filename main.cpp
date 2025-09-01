#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
using namespace std;

#define INF 1e9

class Driver {
public:
    int id;
    int location;
    bool available;
    Driver(int _id, int _location) {
        id = _id;
        location = _location;
        available = true;
    }
};

class Rider {
public:
    int id;
    int pickup;
    int drop;
    Rider(int _id, int _pickup, int _drop) {
        id = _id;
        pickup = _pickup;
        drop = _drop;
    }
};

vector<int> dijkstra(int start, vector<vector<pair<int,int>>> &graph) {
    int n = graph.size();
    vector<int> dist(n, INF);
    dist[start] = 0;
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
    pq.push({0, start});

    while(!pq.empty()) {
        int u = pq.top().second;
        int d = pq.top().first;
        pq.pop();
        if(d > dist[u]) continue;
        for(auto edge : graph[u]) {
            int v = edge.first;
            int w = edge.second;
            if(dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}

vector<int> Notes = {1000, 500, 200, 100, 50, 20, 10, 5};

vector<int> minCoinsDP(int amount) {
    vector<int> dp(amount + 1, INF);
    vector<int> coinUsed(amount + 1, -1);
    dp[0] = 0;

    for(int i = 1; i <= amount; i++) {
        for(int note : Notes) {
            if(i - note >= 0 && dp[i - note] + 1 < dp[i]) {
                dp[i] = dp[i - note] + 1;
                coinUsed[i] = note;
            }
        }
    }

    vector<int> coins;
    int amt = amount;
    while(amt > 0 && coinUsed[amt] != -1) {
        coins.push_back(coinUsed[amt]);
        amt -= coinUsed[amt];
    }
    return coins;
}

int main() {
    vector<string> locations = {
        "Gulshan", "Banani", "Dhanmondi", "Mirpur", "Motijheel",
        "Uttara", "Mohakhali", "Rampura" , "Farmgate", "Aftabnagar"
    };

    int n = locations.size();

    vector<vector<pair<int,int>>> graph(n);

    auto addRoad = [&](int u, int v, int w){
        graph[u].push_back({v,w});
        graph[v].push_back({u,w});
    };

    addRoad(0,1,3); // Gulshan to Banani to Gulshan
    addRoad(0,6,4); // Gulshan to Mohakhali to Gulshan
    addRoad(1,6,2); // Banani to Mohakhali to Banani
    addRoad(1,3,7); //Banani to Mirpur to Banani
    addRoad(2,3,5); // Dhanmondi to Mirpur to Dhanmondi
    addRoad(2,4,6); // Dhanmondi to Motijheel to Dhanmondi
    addRoad(3,5,8); // Mirpur to Uttara to Mirpur
    addRoad(4,6,5); // Motijheel to Mohakhali to Motijheel
    addRoad(4,7,4); // Motijheel to Rampura to Motijheel
    addRoad(5,7,10);// Uttara  Rampura to Uttara
    addRoad(2,8,4); // Dhanmondi to Farmgate to Dhanmondi
    addRoad(7,8,2); // Farmgate to Rampura to Farmgate

    int dCount;
    cout << "Enter number of drivers: ";
    cin >> dCount;
    vector<Driver> drivers;
    cout << "Enter driver ID and current location (0: Gulshan, 1: Banani, 2: Dhanmondi, 3: Mirpur, 4: Motijheel, 5: Uttara, 6: Mohakhali, 7: Rampura, 8: Farmgate):\n";
    for(int i=0;i<dCount;i++){
        int id, loc;
        cin >> id >> loc;
        drivers.push_back(Driver(id, loc));
    }
    int rCount;
    cout << "Enter number of ride requests: ";
    cin >> rCount;
    vector<Rider> riders;
    cout << "Enter rider ID, pickup location, drop location (0 to 7 as above):\n";
    for(int i=0;i<rCount;i++){
        int id, p, d;
        cin >> id >> p >> d;
        riders.push_back(Rider(id,p,d));
    }

    cout << "\n Ride Assignments \n";
    for(auto &rider : riders){
        int minDist = INF;
        int assignedDriver = -1;

        // Find nearest available driver
        for(auto &driver : drivers){
            if(!driver.available) continue;
            vector<int> distToPickup = dijkstra(driver.location, graph);
            if(distToPickup[rider.pickup] < minDist){
                minDist = distToPickup[rider.pickup];
                assignedDriver = driver.id;
            }
        }

        if(assignedDriver == -1){
            cout << "No available driver for Rider " << rider.id << "\n";
            continue;
        }

        // Assign driver
        for(auto &driver : drivers){
            if(driver.id == assignedDriver){
                driver.available = false;
                driver.location = rider.drop; // update after drop
                break;
            }
        }

        vector<int> distPickupToDrop = dijkstra(rider.pickup, graph);
        int tripDistance = distPickupToDrop[rider.drop];
        int fare = tripDistance * 30; // 30 Tk per km
        vector<int> coins = minCoinsDP(fare);

        cout << "Rider " << rider.id << " assigned to Driver " << assignedDriver
             << " | Pickup: " << locations[rider.pickup]
             << " | Drop: " << locations[rider.drop]
             << " | Distance to pickup: " << minDist << " km"
             << " | Trip distance: " << tripDistance << " km"
             << " | Fare: " << fare << " tk"
             << " | Min notes: ";
        for(int c : coins) cout << c << " ";
        cout << "\n";
    }

    cout << "\nAll rides processed.\n";
    return 0;
}
