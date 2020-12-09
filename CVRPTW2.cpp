#include <iostream>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <math.h>
#include <chrono>
#include <ctime>
#include <random>

class Customer {
    public:
        int id;
        int x;
        int y;
        int demand;
        int ready_time;
        int due_time;
        int service_time;

        Customer(int ID, int X, int Y, int DEMAND, int READY_TIME, int DUE_TIME, int SERVICE_TIME) {
            this->id = ID;
            this->x = X;
            this->y = Y;
            this->demand = DEMAND;
            this->ready_time = READY_TIME;
            this->due_time = DUE_TIME;
            this->service_time = SERVICE_TIME;
        };

        Customer() {
            this->id = 0;
            this->x = 0;
            this->y = 0;
            this->demand = 0;
            this->ready_time = 0;
            this->due_time = 0;
            this->service_time = 0;
        };

        Customer &operator=(const Customer &c) {
            this->id = c.id;
            this->x = c.x;
            this->y = c.y;
            this->demand = c.demand;
            this->ready_time = c.ready_time;
            this->due_time = c.due_time;
            this->service_time = c.service_time;
            return *this;
        }

        double get_distance(Customer pierwszy){
            return sqrt(pow(pierwszy.get_x() - this->x, 2)+pow(pierwszy.get_y() - this->y, 2));
        }

        int get_id() {
            return this->id;
        }

        int get_x() {
            return this->x;
        }

        int get_y() {
            return this->y;
        }

        int get_demand() {
            return this->demand;
        }

        int get_ready_time() {
            return this->ready_time;
        }

        int get_due_time() {
            return this->due_time;
        }

        int get_service_time() {
            return this->service_time;
        }

        void print() { 
            std::cout << x << " " << y << " " << demand << " " << ready_time << " " << due_time << " " << service_time << std::endl;
        }

    private:
        
};

struct result {
    int count_routes;
    double routes_sum;
    std::vector< std::vector<int> > routes;
};

class CVRPTW {
    public:
        CVRPTW(int VEHICLE_NUMBER, int VEHICLE_CAPACITY) {
            this->vehicle_number = VEHICLE_NUMBER;
            this->vehicle_capacity = VEHICLE_CAPACITY;
        };

        void add_depot(Customer &c) {
            depot = c;
        }

        void get_depot() {
            std::cout << this->depot.get_x() << " " << this->depot.get_y() << " " << this->depot.get_due_time() << std::endl;
        }

        void add_customer(Customer customer) {
            this->all_customers.push_back(customer);
        }

        Customer find_customer_by_id(int id) {
            return this->all_customers[id - 1];
        }

        bool isValid(){
            for(Customer x: all_customers){
                if(x.get_distance(this->depot) >= x.get_ready_time()){
                    if(x.get_distance(this->depot) * 2 + x.get_service_time() > this->depot.get_due_time()){
                        return false;
                    }
                } else{
                    if(x.get_ready_time() + x.get_service_time() + x.get_distance(this->depot) > this->depot.get_due_time()){
                        return false;
                    }
                }
            }
            return true;
        }

        

        result greedy_solve() {
            if(!isValid()) {
                std::cout << -1 << std::endl;
                return (result){-1, -1, std::vector<std::vector<int> >()};
            }

            std::vector<Customer> customers(this->all_customers);

            int count_routes = 0;
            int count_routes2 = 0;
            double route_cost_sum = 0.0;
            double current_time = 0.0;
            double current_capacity = this->vehicle_capacity;
            Customer current_customer = this->depot;
            std::vector< std::vector<int> > routes;
            std::vector<int> row;
            routes.push_back(row);
            
            while(1) {
                if (customers.empty()) {
                    for(int i = 0; i<count_routes; i++){
                        if(!routes[i].empty()){
                            count_routes2++;
                        }
                    }
                    // std::cout.precision(5);
                    // std::cout << count_routes2 << " " << std::fixed << route_cost_sum << std::endl;
                    // for(int i = 0; i < count_routes2; i++) {
                    //      for(int n : routes[i]) {
                    //          std::cout << n << " ";
                    //      }
                    //      std::cout << std::endl;
                    // }

                    while(routes.back().empty())
                        routes.pop_back();

                    return (result){count_routes2, route_cost_sum, routes};                 
                }

                // Greedily choose next customer
                int next_customer_index = 0;
                for(int i = 1; i < customers.size(); i++) {
                    if(willArriveOnTime(current_customer, customers[i], current_time) && 
                       hasEnoughCapacityFor(customers[i], current_capacity) && 
                       ableToReturnToDepot(current_customer, customers[i], current_time) && 
                       cost_function(current_customer, customers[i], current_time) < cost_function(current_customer, customers[next_customer_index], current_time)) {
                            next_customer_index = i;
                    }
                }

                if (next_customer_index == 0 &&
                    (!willArriveOnTime(current_customer, customers[0], current_time) || 
                    !hasEnoughCapacityFor(customers[0], current_capacity)|| 
                    !ableToReturnToDepot(current_customer, customers[0], current_time))) {
                        next_customer_index = -1;
                }

                // Update variables according to next customer's info
                if (next_customer_index != -1) {
                    routes[count_routes].push_back(customers[next_customer_index].get_id());

                    current_time += current_customer.get_distance(customers[next_customer_index]);
                    route_cost_sum += current_customer.get_distance(customers[next_customer_index]);

                    // If vehicle arrived before customer's ready time -> wait
                    if (current_time < customers[next_customer_index].get_ready_time()) {
                        route_cost_sum += (customers[next_customer_index].get_ready_time() - current_time);
                        current_time = customers[next_customer_index].get_ready_time();
                    }

                    current_time += customers[next_customer_index].get_service_time();
                    route_cost_sum += customers[next_customer_index].get_service_time();

                    current_capacity -= customers[next_customer_index].get_demand();
                    current_customer = customers[next_customer_index];
                
                    // Remove chosen customer from customer list
                    customers.erase(customers.begin() + next_customer_index);
                    
                }

                // If no suitable customer was found or vehicle's capacity is equal to zero
                if (next_customer_index == -1 || current_capacity == 0) {
                    count_routes++;
                    route_cost_sum += current_customer.get_distance(this->depot);
                    current_time = 0.0;
                    current_capacity = this->vehicle_capacity;
                    current_customer = this->depot;
                    std::vector<int> row;
                    routes.push_back(row);
                }

                if(customers.empty()){
                    count_routes++;
                    route_cost_sum += current_customer.get_distance(this->depot);
                }
            }
        }
        
        bool is_route_valid(std::vector<int> route) {
            double current_time = 0;
            int sum_of_demand = 0;
            Customer current_customer = this->depot;

            for(int id : route) {
                Customer next_customer = find_customer_by_id(id); 

                current_time += current_customer.get_distance(next_customer);
                sum_of_demand += current_customer.get_demand();
                if (current_time < next_customer.get_ready_time()) 
                    current_time = next_customer.get_ready_time();
                else if (current_time > next_customer.get_due_time())
                    return false;

                current_time += next_customer.get_service_time();
                current_customer = next_customer;
            }

            if(current_time + current_customer.get_distance(this->depot) < this->depot.get_due_time())
                if (sum_of_demand <= this->vehicle_capacity)
                    return true;

            return false;
        }

        std::vector<int> two_opt_swap(std::vector<int> route, int j, int k) {
            std::vector<int> new_route;

            // 1. take route[0] to route[j-1] and add them in order to new_route
            for (int i = 0; i <= j - 1; i++)
                new_route.push_back(route[i]);

            // 2. take route[j] to route[k] and add them in reverse order to new_route
            for (int i = k; i >= j; i--)
                new_route.push_back(route[i]);

            // 3. take route[k+1] to end and add them in order to new_route
            for (int i = k + 1; i < route.size(); i++)
                new_route.push_back(route[i]);

            // 4. check new_route validity
            if (is_route_valid(new_route)) {
                return new_route;
            }

            return route;
        }

        std::vector<int> two_opt(std::vector<int> route) { 
            std::vector<int> newRoute(route);
            while(true) {
                double bestCost = getRouteCost(route);
                bool isImprovement = false;
                for(int i = 0; i < route.size() - 1; i++) {
                    for(int j = i + 1; j < route.size(); j++) {
                        std::vector<int> newRoute = two_opt_swap(route, i, j);
                        double newCost = getRouteCost(newRoute);
                        if(newCost < bestCost) {
                            isImprovement = true;
                            route = newRoute;
                            bestCost = newCost;
                        }
                    }
                }

                if(isImprovement == false) return newRoute; 
            }
        }

        int exchange_between(std::vector<int>& route1, std::vector<int>& route2) {
            // Exchange customer betweeen two routes
            std::random_device rd;  //Will be used to obtain a seed for the random number engine
            std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
            std::uniform_real_distribution<> dis(0.0, 1.0);
            int last_i = -1;
            int last_j = -1;

            for(int i = 0; i<route1.size(); i++){
                for(int j = 0; j<route2.size(); j++){
                    std::swap(route1[i],route2[j]);
                    if( is_route_valid(route1) && is_route_valid(route2)){
                        if( dis(gen) <= 0.15) {
                            return 1;
                        }
                        last_i = i;
                        last_j = j;
                    }
                    std::swap(route1[i],route2[j]);
                }
            }
            if(last_i != -1){
                std::swap(route1[last_i], route2[last_j]);
                return 1;
            }
            return -1;
        }
        
        double getRouteCost(std::vector<int> route) {
            double current_time = 0;
            Customer current_customer = this->depot;

            for(int id : route) {
                Customer next_customer = find_customer_by_id(id); 

                current_time += current_customer.get_distance(next_customer);
                if (current_time < next_customer.get_ready_time()) 
                    current_time = next_customer.get_ready_time();

                current_time += next_customer.get_service_time();
                current_customer = next_customer;
            }

            return current_time + current_customer.get_distance(this->depot);
        }

        double getResultCost(std::vector< std::vector<int> > routes) {
            double cost = 0;
            for(std::vector<int> route : routes) {
                cost += getRouteCost(route);
            }

            return cost;
        }

        std::vector<result> getNeighbours(result bestCandidate, int neighbourhoodSize) {
            std::vector<result> neighbourhood;

            for(int i = 0; i < neighbourhoodSize; i++) {
                // coś się dzieje
                std::vector< std::vector<int> > neighbour(bestCandidate.routes);
                std::random_device rd;  
                std::mt19937 gen(rd()); 
                std::uniform_int_distribution<> dis(0, neighbour.size() - 1);

                int randomIdx1 = dis(gen);
                int randomIdx2 = dis(gen);
                
                exchange_between(neighbour[randomIdx1], neighbour[randomIdx2]);

                // if (exchange_between(neighbour[randomIdx1], neighbour[randomIdx2]) == 1) {
                //     std::cout << "dobrze" << std::endl;
                // } else {
                //     std::cout << "niedobrze" << std::endl;
                // }

                two_opt(neighbour[randomIdx1]);
                two_opt(neighbour[randomIdx2]);

                // for(std::vector<int> route : neighbour) {
                //     two_opt(route);
                // }

                neighbourhood.push_back((result){(int)neighbour.size(), getResultCost(neighbour), neighbour});
            }
            return neighbourhood;
        }

        bool isInTabu(std::vector<result> tabu, result candidate) {
            for(result tabuItem: tabu) {
                if((tabuItem.count_routes == candidate.count_routes) &&
                    (tabuItem.routes_sum == candidate.routes_sum) &&
                    (tabuItem.routes == candidate.routes)) // moze nie dzialac
                        return true;
            }
            return false;
        }

        result tabu_search_solve() {
            // Get greedy solution 
            result current_best = greedy_solve();
            result best_candidate = current_best;
            std::vector<result> tabu = {current_best};
            int neighbourhoodSize = 20; // to be adjusted
            int max_tabu_size = 20; // to be adjusted

            bool stop = false; // swap with time constraint

            time_t start = time(0);

            while(true) {
                std::vector<result> neighbourhood = getNeighbours(best_candidate, neighbourhoodSize);

                best_candidate = neighbourhood[0];

                for(result possible_candidate: neighbourhood) {
                    if(!isInTabu(tabu, possible_candidate) && possible_candidate.routes_sum < best_candidate.routes_sum) {
                        best_candidate = possible_candidate;
                    }
                }

                if (best_candidate.routes_sum < current_best.routes_sum) {
                    current_best = best_candidate;
                }

                tabu.push_back(best_candidate);
                if (tabu.size() > max_tabu_size) {
                    tabu.erase(tabu.begin());
                }
                
                time_t end = time(0);

                if(end - start > 30.0) {
                    break;
                }
            }

            return current_best;
        }

    private:
        int vehicle_number;
        int vehicle_capacity;
        std::vector<Customer> all_customers;
        Customer depot;

        bool willArriveOnTime(Customer current, Customer next, double current_time) {
            return current_time + current.get_distance(next) <= next.get_due_time();
        }

        bool hasEnoughCapacityFor(Customer c, int current_capacity) {
            return current_capacity >= c.get_demand();
        }

        bool ableToReturnToDepot(Customer current, Customer next, double current_time) {
            double time = current_time + current.get_distance(next);
            time = (time < next.get_ready_time()) ? next.get_ready_time() : time;
            time += next.get_service_time();

            return this->willArriveOnTime(next, this->depot, time);
        }

        double cost_function(Customer current, Customer next, double current_time) {
            return (current.get_distance(next)) + (next.get_due_time() - current_time);
        }
};

int main(int argc, char* argv[])
{
    
    
    // if(argc != 3){
    //     printf("usage: CVRPTW.exe input file output file\n");
    //     return -1;
    // }

    std::ifstream example_input;
    example_input.open(argv[1]);
    std::string line;
    
    //skipping blank or unimportant lines, getting the capacity of the vehicles
    for(int i = 0; i < 4; i++)
    {
        std::getline(example_input, line);
        if(line.length()<4){
            i--;
        }
    }
    std::stringstream s(line);
    int vehicle_n, vehicle_c;
    s >> vehicle_n >> vehicle_c;
    CVRPTW problem = CVRPTW(vehicle_n, vehicle_c);
    Customer depot;

    //skipping blank or unimportant lines
    for(int i = 0; i < 2; i++)
    {
        std::getline(example_input, line);
        if(line.length()<4){
            i--;
        }
    }

    int customer_num = -1;
    int x;
    int y;
    int dem;
    int ready;
    int due;
    int service;

    while (std::getline(example_input, line)) {
        if((line.length()<4) && (customer_num != -1)){
            break;
        }
        if((customer_num==-1) && (line.length()<6)){

            std::getline(example_input,line);
        }
        std::stringstream s(line);
        s >> customer_num >> x >> y >> dem >> ready >> due >> service;
        if(customer_num == 0) {
            depot = Customer(customer_num, x, y, dem, ready, due, service);
            problem.add_depot(depot);
        } else {
            problem.add_customer(Customer(customer_num, x, y, dem, ready, due, service));
        }
    }
    
    // result greedy_answer = problem.greedy_solve();
    result tabu_answer = problem.tabu_search_solve();

    // std::cout << greedy_answer.count_routes << " " << greedy_answer.routes_sum << std::endl;

    std::cout << tabu_answer.count_routes << " " << tabu_answer.routes_sum << std::endl;

    for(std::vector<int> route: tabu_answer.routes) {
        for (int customer: route) {
            std::cout << customer << " ";
        }
        std::cout << std::endl;
    }

    example_input.close();
    
    return 0;
}
