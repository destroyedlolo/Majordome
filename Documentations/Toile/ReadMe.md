Toile
=====

Toile is an optional module to create smart dashboards based on [Séléné](https://github.com/destroyedlolo/Selene) rendering plugins

# Objects handled by Toile

- **Renderer** : Link to physical devices to build user interfaces (UI).
- **Painting** : a painting surface that having its own memory.
- **Decoration** : task to be called every time a renderer/painting needs to be refreshed.
- **Series** : list of paintings to be displayed.
- **Field** : simple text field.

> [!TIP]
> It is possible to write on a display's primary surface (everything you write is immediately reflected on the screen) : you have to use APIs associated with the renderer.
>  In such cases, you will lose the advanced capabilities like layering, surface disabling, ... Useful with simple interfaces.
