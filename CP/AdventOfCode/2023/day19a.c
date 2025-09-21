#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define MAX_RULES_PER_WORKFLOW 10
#define MAX_WORKFLOWS 600
#define MAX_PARTS 300
#define MAX_NAME_LEN 10

typedef struct {
  char category;
  char op;
  int value;
  char destination[MAX_NAME_LEN];
} Rule;

typedef struct {
  char name[MAX_NAME_LEN];
  Rule rules[MAX_RULES_PER_WORKFLOW];
  int rule_count;
} Workflow;

typedef struct {
  int x, m, a, s;
} Part;

Workflow workflows[MAX_WORKFLOWS];
Part parts[MAX_PARTS];
int workflow_count = 0;
int part_count = 0;

void parse_line(char *line) {
  if (line[0] == '{') { // Parsing a part
    sscanf(line, "{x=%d,m=%d,a=%d,s=%d}", &parts[part_count].x,
           &parts[part_count].m, &parts[part_count].a, &parts[part_count].s);
    part_count++;
  } else if (strlen(line) > 1) { // Parsing a workflow
    Workflow *wf = &workflows[workflow_count];
    char *name_end = strchr(line, '{');
    strncpy(wf->name, line, name_end - line);
    wf->name[name_end - line] = '\0';

    char *rules_str = name_end + 1;
    rules_str[strlen(rules_str) - 1] = '\0'; // Remove trailing '}'

    char *rule_token = strtok(rules_str, ",");
    while (rule_token != NULL) {
      Rule *rule = &wf->rules[wf->rule_count];
      char *colon = strchr(rule_token, ':');

      if (colon) { // Conditional rule
        sscanf(rule_token, "%c%c%d:%s", &rule->category, &rule->op,
               &rule->value, rule->destination);
      } else {              // Default rule
        rule->category = 0; // Sentinel for default
        strcpy(rule->destination, rule_token);
      }
      wf->rule_count++;
      rule_token = strtok(NULL, ",");
    }
    workflow_count++;
  }
}

int find_workflow_index(const char *name) {
  for (int i = 0; i < workflow_count; i++) {
    if (strcmp(workflows[i].name, name) == 0) {
      return i;
    }
  }
  return -1; // Not found
}

bool process_part(const Part *part) {
  char current_workflow[MAX_NAME_LEN] = "in";
  while (strcmp(current_workflow, "A") != 0 &&
         strcmp(current_workflow, "R") != 0) {
    int wf_index = find_workflow_index(current_workflow);
    const Workflow *wf = &workflows[wf_index];
    bool rule_matched = false;

    for (int i = 0; i < wf->rule_count; i++) {
      const Rule *rule = &wf->rules[i];
      bool condition_met = false;
      if (rule->category == 0) {
        condition_met = true;
      } else {
        int part_value;
        switch (rule->category) {
        case 'x':
          part_value = part->x;
          break;
        case 'm':
          part_value = part->m;
          break;
        case 'a':
          part_value = part->a;
          break;
        case 's':
          part_value = part->s;
          break;
        }
        if (rule->op == '<' && part_value < rule->value)
          condition_met = true;
        else if (rule->op == '>' && part_value > rule->value)
          condition_met = true;
      }

      if (condition_met) {
        strcpy(current_workflow, rule->destination);
        rule_matched = true;
        break;
      }
    }
    if (!rule_matched) {
      // No rule matched, break to avoid infinite loop
      return false;
      break;
    }
  }
  return strcmp(current_workflow, "A") == 0;
}

int main() {
  FILE *file = fopen("input.txt", "r");
  if (!file) {
    perror("Could not open input.txt");
    return 1;
  }

  char line[512];
  while (fgets(line, sizeof(line), file)) {
    // Strip newline character
    line[strcspn(line, "\n")] = 0;
    parse_line(line);
  }
  fclose(file);

  long long total_sum = 0;
  for (int i = 0; i < part_count; i++) {
    if (process_part(&parts[i])) {
      total_sum += parts[i].x + parts[i].m + parts[i].a + parts[i].s;
    }
  }

  printf("Total sum of ratings for accepted parts: %lld\n", total_sum);

  return 0;
}
