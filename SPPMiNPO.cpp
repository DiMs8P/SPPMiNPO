#include <iostream>
#include <random>

struct HubertDistributionSettings {
    double v;
    double k;
    double variance;
    double gamma;
    double P;
};

class HubertDistribution {
public:
    HubertDistribution(HubertDistributionSettings&& other) : settings(other), gen(std::random_device()()), dis(0.0, 1.0) {}

    double get() {
        double r1 = dis(gen);
        if (r1 < settings.P) {
            return first_random_value();
        }

        return second_random_value(r1);
    }

private:
    double first_random_value() {
        double x1 = 0;
        std::normal_distribution<double> distribution(0, 1);

        do {
            x1 = distribution(gen);
        } while (std::abs(x1) > settings.k);

        return x1;
    }

    double second_random_value(double r1) {
        const double r2 = dis(gen);
        const double x2 = settings.k - (std::log(r2) / settings.k);

        return r1 < (1 + settings.P) / 2 ? x2 : -x2;
    }

protected:
    HubertDistributionSettings settings;
    std::mt19937 gen;
    std::uniform_real_distribution<> dis;
};

int main()
{
    HubertDistribution hubertDistribution({ 0.05, 1.398, 1.41, 1.52, 0.796 });
    for (int i = 0; i < 10; i++) {
        std::cout << hubertDistribution.get() << std::endl;
    }

    return 0;
}