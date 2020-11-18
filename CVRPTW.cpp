#include <iostream>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <math.h>

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

bool compare_due_time(Customer a, Customer b) {
    return a.get_due_time() < b.get_due_time();
}

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
            this->customers.push_back(customer);
        }

        int check_validity(){
            for(auto x: customers){
                if ((x.get_x()==depot.get_x()) && (x.get_y()==this->depot.get_y())){
                    continue;
                }
                if(x.get_distance(this->depot) >= x.get_ready_time()){
                    if(x.get_distance(this->depot)*2 + x.get_service_time() > this->depot.get_due_time()){
                        return -1;
                    }
                }
                else{
                    if(x.get_ready_time()+x.get_service_time()+x.get_distance(this->depot) > this->depot.get_due_time()){
                        return -1;
                    }
                }
            }
            return 1;
        }

        int find_closest_customer(Customer current_customer) {
            int closest_index = 0;
            for(int i = 0; i < customers.size(); i++) {
                if(current_customer.get_distance(customers[i]) < current_customer.get_distance(customers[closest_index])) {
                    closest_index = i;
                }
            }
            return closest_index;
        }

        void greedy_solve() {
            // if(check_validity() == -1) {
            //     std::cout << -1 << std::endl;
            //     return;
            // }

            int count_routes = 0;
            double route_cost_sum = 0.0;
            double current_time = 0.0;
            double current_capacity = this->vehicle_capacity;
            Customer current_customer = this->depot;
            std::vector< std::vector<int> > routes;
            std::vector<int> row;
            routes.push_back(row);

            // std::sort(customers.begin(), customers.end(), compare_due_time);

            while(1) {
                if (customers.empty()) {
                    std::cout << count_routes << " " << std::setprecision(5) << route_cost_sum << std::endl;
                    // for(int i = 0; i < count_routes; i++) {
                    //     for(int n : routes[i]) {
                    //         std::cout << n << " ";
                    //     }
                    //     std::cout << std::endl;
                    // }
                    break;
                }

                // // Greedily choose next customer
                // int next_customer_index = -1;
                // for(int i = 0; i < customers.size(); i++) {
                //     if((current_time + current_customer.get_distance(customers[i]) <= customers[i].get_due_time()) && (current_capacity >= customers[i].get_demand())) {
                //         next_customer_index = i;
                //         break;
                //     }
                // }

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
            }
        }

    private:
        int vehicle_number;
        int vehicle_capacity;
        std::vector<Customer> customers;
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
            return current.get_distance(next);
            // return next.get_due_time() - current_time;
            // return (current.get_distance(next)) * (next.get_due_time() - current_time);
            // return (current.get_distance(next)) + (next.get_due_time() - current_time);
        }
};

int main()
{
    std::ifstream example_input;
    example_input.open("./Input/m2kvrptw-0.txt");
    std::string line;

    //pomijanie niewaznych linijek i zczytanie wlasnosci samochodu
    for(int i = 0; i < 4; i++)
    {
        std::getline(example_input, line);
        if(line.length()<6){
            i--;
        }
    }

    std::stringstream s(line);
    int vehicle_n, vehicle_c;
    s >> vehicle_n >> vehicle_c;
    std::cout << vehicle_n << " " << vehicle_c << std::endl;
    CVRPTW problem = CVRPTW(vehicle_n, vehicle_c);
    Customer depot;

    //pomijanie niewaznych linijek
    for(int i = 0; i < 2; i++)
    {
        std::getline(example_input, line);
        if(line.length()<6){
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
        if((line.length()<6) && (customer_num != -1)){
            break;
        }
        if((customer_num==-1) && (line.length()<6)){
            std::cout << "elo" << std::endl;
            std::getline(example_input,line);
        }
        std::stringstream s(line);
        s >> customer_num >> x >> y >> dem >> ready >> due >> service;
        std::cout << customer_num << std::endl;
        if(customer_num == 0) {
            depot = Customer(customer_num, x, y, dem, ready, due, service);
            problem.add_depot(depot);
        } else {
            problem.add_customer(Customer(customer_num, x, y, dem, ready, due, service));
        }
    }

    //problem.greedy_solve();
    example_input.close();
    return 0;
}
