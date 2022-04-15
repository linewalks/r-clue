
devtools::document()
devtools::load_all()

library(rclue)
# library(Rcpp)

clue <- new(CLUEWrapper, "192.168.0.69", 8889, "test@linewalks.com", "q1w2e3r4!")

print(clue)

conn <- clue$Connect()
print(conn)

stream <- conn$GetIncidenceRateRaw(282)
print(stream)
print(stream$Fetch(5))
# print(stream$FetchAll())
stream$Close()

# res <- conn$GetIncidenceRateResult(282)
# print(res)

# res <- conn$GetCohortComparison(1750)
# print(res)

# res <- conn$GetCohortList(1, 10, "")
# print(res)

# stream <- conn$GetCohortPersonTable(527)
# print(stream)
# print(stream$Fetch(10))
# print(stream$Fetch(5))
# stream$Close()

# Sys.sleep(1)

# stream <- conn$GetCohortDeathTable(527)
# print(stream$Fetch(10))
# stream$Close()
