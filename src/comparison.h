#ifndef COMPARISON_H
#define COMPARISON_H
#include "stream.h"

using namespace Rcpp;

DataFrame convert_comparison(ResponseComparison response) {
  int cohort_size = response.cohort_list_size();
  int row_size = response.row_list_size();

  std::vector<ComparisonCohortInfo> cohort_vector;
  std::vector<std::vector<std::string>> values_vector;
  for (int i = 0; i < cohort_size; ++i) {
    ComparisonCohortInfo cohort_info = response.cohort_list()[i];
    cohort_vector.push_back(cohort_info);
    values_vector.push_back(std::vector<string>());
  }

  std::vector<std::string> category1_vector;
  std::vector<std::string> category2_vector;
  std::vector<float> p_value_vector;
  std::vector<bool> p_value_is_group_vector;
  for (int i = 0; i < row_size; ++i) {
    ComparisonRowInfo row_info = response.row_list()[i];
    category1_vector.push_back(row_info.category1());
    category2_vector.push_back(row_info.category2());

    for (int j = 0; j < cohort_size; ++j) {
      std::string value_str = row_info.values()[j];
      values_vector[j].push_back(value_str);
    }

    p_value_vector.push_back(row_info.p_value());
    p_value_is_group_vector.push_back(row_info.p_value_is_group());
  }

  DataFrame df = DataFrame::create();
  df.push_back(category1_vector, "category1");
  df.push_back(category2_vector, "category2");
  for (int i = 0; i < cohort_size; ++i) {
    ComparisonCohortInfo cohort_info = cohort_vector[i];
    df.push_back(values_vector[i], cohort_info.name());
  }
  df.push_back(p_value_vector, "p_value");
  df.push_back(p_value_is_group_vector, "p_value_is_group");
  return df;
}
#endif
