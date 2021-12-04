Solutions of Advent of Code (which at least worked with my input files)

### Regarding the C-code from 2020
Please note that at some point I didn't properly free all the memory I'm using
in the end. In most cases it shouldn't be too hard to add this, but I didn't
adust up to now. Just wanted to note that I consider this bad style (even though
the space is freed by the OS anyway in the end).

While running there shouldn't be no memory leaks, since I free space with no
pointer on it normally. But don't be too sure since this isn't checked with a
sanitizer or valdrind, so maybe I missed some.

Also note that I used the header files implementing some datastructured from the
bsd libs (`tree` and `queue`).

# Download your input files by script
The script [`get_input.lua`](get_input.lua) can retrive your puzzle input if you run it with a
`.sess` file only holding your sessionkey. It will store the result in
`day<dayNr>.dat`. By default the input of the current day is pulled, but you may
specify `<day> <year>` as commandline options. There are NO checks if the `day<dayNr>.dat`
file is already present (would be overwritten).

## Dependencies
- [luasec](https://luarocks.org/modules/brunoos/luasec)
