Toile
=====

Toile is an optional module to create smart dashboards based on [Séléné](https://github.com/destroyedlolo/Selene) rendering plugins

# Objects handled by Toile

- **[Renderer](Renderer.md)** : Generic link to physical devices to build user interfaces (UI).
  - **[LCD](LCD.md)** : A renderer for tiny I2C textual LCD screen, avoiding the need of low level coding.
- **[Painting](Painting.md)** : Subpart of a surface to paint on.
- **[Decoration](Decoration.md)** : task to be called every time a renderer/painting needs to be refreshed.
<!--
- **Series** : list of paintings to be displayed.
-->
- **Field** : simple text field.

> [!TIP]
> It is possible to write on a display's primary surface (everything you write is immediately reflected on the screen) : you have to use APIs associated with the renderer.
>  In such cases, you will lose the advanced capabilities like layering, surface disabling, ... Useful with simple interfaces.

# Directive known by (almost) all Toile's object

#### -->> hidden

The object is not visible
