ENTITIES

App:
    windows
    current_window
    buffers

    load_buffer(file_path)
    run()
    display_windows()

Window:
    rows, cols
    x, y
    buffer
    cursor_line
    cursor_col

    draw()
    handle_key(ch)

Buffer

GapBuffer
Line
Buffer - array of Lines 

TODO
- [x] Memory + dynamic array 
- [x] Window
- [x] Move cursor
- [x] Gap Buffer
- [ ] Window rendering, scroll + cursor position
- [ ] Windows, splits, floats, textarea rect, number lines, statusbar
- [ ] Many windows and buffers
- [ ] Read/write files
- [ ] Visual mode, copy, paste
- [ ] Line wrapping
- [ ] Syntax highlight
- [ ] Search
- [ ] Vim motions
- [ ] Configs
- [ ] Rope

IDEAS
- embed lua?

