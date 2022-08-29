# Hospital-Allocating-System
This system allocates hosiptal to clients based on distance between them and the availability of the hospital.

## Objective

This is a distributed system design where the scheduler is further connected to three hospitals. Each hospital stores its own capacity, availability and the map. Since users and hospitals are all in the same city, they will share the same map. Therefore, once the scheduler receives a client query, it decodes the query and further sends a request to all the appropriate hospitals. Each of the hospitals will calculate a score according to its own availability and the distance to the client. Then, each hospital will reply to the scheduler. Finally, the scheduler will assign the client to the hospital with the highest score, and inform the client and the hospitals of the decision.
