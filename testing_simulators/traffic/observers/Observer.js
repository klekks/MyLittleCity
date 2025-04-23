export class Observer
{
    on_event(object, event)
    {
        console.error(`Using pure virtual method Observer with {object}:{event}`);
    }
};


export class ObserverSubject
{
    constructor()
    {
        this.observers = new Set;
    }

    add_observer(observer)
    {
        this.observers.add(observer);
    }

    remove_observer(observer)
    {
        this.observers.delete(observer);
    }

    notify(object, event)
    {
        this.observers.forEach(observer => observer.on_event(object, event));
    }
};

