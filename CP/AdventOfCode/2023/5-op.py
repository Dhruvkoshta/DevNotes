def parse_input_file(filepath):
    """
    Parses the input file to extract seeds and all mapping tables.
    """
    with open(filepath) as f:
        data = f.read().strip()

    sections = data.split('\n\n')
    
    # Parse seeds as a list of (start, length) tuples
    seeds_line = sections[0].split(': ')[1]
    seed_numbers = [int(num) for num in seeds_line.split()]
    
    seed_ranges = []
    for i in range(0, len(seed_numbers), 2):
        seed_ranges.append((seed_numbers[i], seed_numbers[i+1]))
    
    # Parse maps
    parsed_maps = []
    for section in sections[1:]:
        lines = section.split('\n')[1:]
        current_map = []
        for line in lines:
            current_map.append(tuple(map(int, line.split())))
        parsed_maps.append(current_map)

    return seed_ranges, parsed_maps


def apply_mapping_to_ranges(input_ranges, mapping):
    """
    Applies a single mapping table to a set of input ranges.
    Returns the new, transformed ranges.
    """
    output_ranges = []
    
    # Use a list of ranges that still need to be processed
    # We start with the full set of input ranges
    ranges_to_process = list(input_ranges)
    
    # Iterate through each entry in the mapping
    for dest_start, src_start, map_len in mapping:
        processed_ranges = []
        next_ranges_to_process = []
        
        # Check all the ranges we have left to process
        for r_start, r_len in ranges_to_process:
            r_end = r_start + r_len
            src_end = src_start + map_len
            
            # Case 1: The map range is outside the seed range
            before_start = (r_start, min(r_len, src_start - r_start))
            if before_start[1] > 0:
                next_ranges_to_process.append(before_start)

            # Case 2: The map range overlaps with the seed range
            overlap_start = max(r_start, src_start)
            overlap_end = min(r_end, src_end)
            if overlap_end > overlap_start:
                overlap_len = overlap_end - overlap_start
                mapped_start = dest_start + (overlap_start - src_start)
                processed_ranges.append((mapped_start, overlap_len))

            # Case 3: The map range is after the seed range
            after_start = (max(r_start, src_end), r_end - max(r_start, src_end))
            if after_start[1] > 0:
                next_ranges_to_process.append(after_start)
        
        ranges_to_process = next_ranges_to_process
        output_ranges.extend(processed_ranges)
        
    output_ranges.extend(ranges_to_process)
    
    return output_ranges

def main():
    """
    Main function to find the lowest location number.
    """
    seed_ranges, maps = parse_input_file("input.txt")
    print(f"Initial seed ranges: {seed_ranges}")
    
    # Pass the ranges through each map sequentially
    current_ranges = seed_ranges
    for mapping in maps:
        current_ranges = apply_mapping_to_ranges(current_ranges, mapping)
        
    # Find the minimum start of all final ranges
    final_min = float('inf')
    if current_ranges:
        final_min = min(start for start, _ in current_ranges)
    else:
        print("Error: No final ranges found.")

    print(f"\nFinal minimum location: {final_min}")

if __name__ == "__main__":
    main()
