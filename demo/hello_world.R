
devtools::document()
devtools::load_all()

library(rclue)
# library(Rcpp)

clue <- new(CLUEWrapper, "localhost", 9999, "test@linewalks.com", "q1w2e3r4!")

print(clue)

conn <- clue$Connect()
print(conn)

res <- conn$GetCohortList(1, 10, "")
print(res)
