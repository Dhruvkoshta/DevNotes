count = 0
list_of_lists = []

file_path = "input.txt" # Assuming 'input.txt' exists with numbers separated by spaces

with open(file_path, 'r') as file:
    for line in file:
        cleaned_line = line.strip()
        
        if cleaned_line:
            int_list = [int(item) for item in cleaned_line.split(" ")]
            list_of_lists.append(int_list)

            is_non_decreasing = True
            is_non_increasing = True

            for i in range(len(int_list) - 1):
                if int_list[i] > int_list[i+1]:
                    is_non_decreasing = False
                if int_list[i] < int_list[i+1]:
                    is_non_increasing = False

            if is_non_decreasing or is_non_increasing:
                count += 1

print(f"List of lists: {list_of_lists}")
print(f"Number of monotonic lists: {count}")
