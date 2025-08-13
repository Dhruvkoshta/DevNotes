def lookup(thing, mapping):
    """
    Finds the mapped value for a given 'thing' based on a list of maps.
    """
    for dest_start, src_start, length in mapping:
        if src_start <= thing < src_start + length:
            return dest_start + (thing - src_start)
    return thing

def parse_input_file(filepath):
    """
    Parses the input file to extract seeds and all mapping tables.
    """
    with open(filepath) as f:
        data = f.read().strip()

    sections = data.split('\n\n')
    
    # Parse seeds
    seeds_line = sections[0]
    seed_numbers = [int(num) for num in seeds_line.split(': ')[1].split()]
    
    # Parse maps
    parsed_maps = []
    for section in sections[1:]:
        lines = section.split('\n')[1:] # Skip the "map:" line
        current_map = []
        for line in lines:
            current_map.append(tuple(map(int, line.split())))
        parsed_maps.append(current_map)

    return seed_numbers, parsed_maps

def main():
    """
    Main function to run the optimization logic.
    """
    seed_array, maps = parse_input_file("input.txt")
    print(f"Seeds: {seed_array}")

    # The most crucial optimization: Process seeds by ranges.
    # We no longer generate a massive list of individual seeds.
    final_min = float('inf')

    # Iterate over the seed ranges from PART B
    for i in range(0, len(seed_array), 2):
        start_seed = seed_array[i]
        range_length = seed_array[i+1]
        
        # Iterate through each seed in the current range
        for j in range(start_seed, start_seed + range_length):
            current_value = j
            for mapping in maps:
                current_value = lookup(current_value, mapping)
            
            if current_value < final_min:
                final_min = current_value

    print(f"\nFinal minimum location: {final_min}")

if __name__ == "__main__":
    main()
