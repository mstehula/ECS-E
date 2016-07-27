<h1> ECS-E </h1>
ECS-E is a Data Oriented Design approach to the Entity Component System, with Events as a means to communicate between Systems. ECS-E moves components, which are typically managed by the Entity in Object Oriented Programming, to the Systems. This allows the Systems to move and reorganize the data, as well as run it in one loop without cache misses, and causes it to run more efficiently.

<h2> Entities/Components/Systems </h2>
The ECS is controlled by 3 handlers or managers, the Entity Manager, the System Manager, and the Component Manager. The Entity and System Managers are at the top of a hierarchy, controlling the Entities and Systems each. They are loosely tied together by a means of several lower level functions, such as to get System ID by Type or get Entity by Global Unique IDentifier (GUID). One Entity Manager manages all of the Entities. One System Manager manages all of the Systems.

The Component Manager is more hidden then the Entity or System Managers, There are also more of them. There is one Component Manager for every System, and they are directly implemented into the System. Between the System Manager, the System and the Component Manager, this is where most of the magic happens for Data Oriented Design.

When adding an Entity at the Entity Manager, it communicates with the System Manager using Events to add Components to each of the Component Managers, with all data stored in the Event. Note that construction/destruction of Entities should happen at either the beginning or the end of the game loop cycle. The Component Manager can then create the new Component, storing the GUID and index where the component is stored into a map based on the GUID. The Component Manager is also able to move components around as it sees fit to make the process smoother. This makes it so that anyone looking for the Component can easily find it by associating it with the Entities GUID, and the System can easily loop over all Components of the same type without many cache misses.

<h2> Events </h2>
Events are the simple but complex way to communicate between all Systems about all Entities. Events must contain a few things, including Entities involved and a timestamp. Events also can contain data, or can simply be a signal saying that something has happened.

All Events are launched through the Event Manager, using a command called Fire(). All Events are then sent to all Systems that are called "Subscribers", which causes the Event to trigger a callback through a function. Events should be asynchronously called but synchronously called back.

There are several built in Events to the ECS-E, most notably are the Entity Created and Entity Destroyed Events. More events can always be created as needed and linked to fire at appropriate times.

<h1> Examples </h1>
