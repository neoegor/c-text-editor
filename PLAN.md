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

Buffer:
    lines

TODO
- [x] Memory + dynamic array 
- [x] Window
- [x] Move cursor
- [ ] Gap Buffer
