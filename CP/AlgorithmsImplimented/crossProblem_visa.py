def main(matrix):
    R = len(matrix)
    C = len(matrix[0])

    total = 0

    for r in range(R):
        for c in range(C):
            elementsToCheck = []
            for j in range(C):
                if j != c:
                    elementsToCheck.append(matrix[r][j])
            for i in range(R):
                if i != r:
                    elementsToCheck.append(matrix[i][c])
            if not elementsToCheck:
                total += 1
            else:
                is_current_good = True
                expected = elementsToCheck[0]
                for val in elementsToCheck:
                    if val != expected:
                        is_current_good = False
                        break
                if is_current_good:
                    total += 1
    print(total)
    return total

if __name__ == "__main__":
    

    matrix = [[1, 1, 1, 1], [2, 3, 1, 1], [1, 1, 1, 0], [1, 4, 1, 1]]
    main(matrix)
