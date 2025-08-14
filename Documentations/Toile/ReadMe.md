Toile
=====

Toile is an optional module to create smart dashboards based on [Séléné](https://github.com/destroyedlolo/Selene) rendering plugins

# Objects handled by Toile
## renderers

> [!TIP]
> It is possible to write on a display's primary surface (everything you write is immediately reflected on the screen) : you have to use APIs associated with the renderer.
>  In such cases, you will lose the advanced capabilities like layering, surface disabling, ... Useful with simple interfaces.

- **[Renderer](Renderer.md)** : Generic link to physical devices to build user interfaces (UI).
  - **[LCD](LCD.md)** : A renderer for tiny I2C textual LCD screen, avoiding the need of low level coding.

## Graphical objects
- **[Decoration](Decoration.md)** : a task to be called every time a renderer/painting needs to be refreshed.
- **[Painting](Painting.md)** : Subpart of a surface to paint on.
  - **Field** : simple text field.

### Graphical Objects Specific Directives
#### -->> ApplyOn Renderer= and -->> ApplyOn Painting=

Indicate the **[Renderer](Renderer.md)** or the **[Painting](Painting.md)** this object bellong to.

#### -->> hidden

The object is not visible at launch time.

<!--
## Management
- **Series** : list of paintings to be displayed.
-->
