#include "ConcreteObserver.hpp"
#include "ConcreteSubject.hpp"
#include "Observer.hpp"
#include "Subject.hpp"

#include <iostream>

auto main() -> int {
    ConcreteObeserver *B = new ConcreteObeserver("B");
    ConcreteObeserver *C = new ConcreteObeserver("C");

    ConcreteSubject *Group = new ConcreteSubject();

    Group->Attach(B);
    Group->Attach(C);

    Group->Notify("hello");

    Group->Detach(C);

    Group->Notify("World");
    delete B;
    delete C;
    delete Group;
    return 0;
}
