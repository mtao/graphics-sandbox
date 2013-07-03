#include "macgridfactory.h"


int main() {
    MACGridFactory<double,3> factory(10,10,10);
    factory.create<mtao::internal::NGrid>();

}
