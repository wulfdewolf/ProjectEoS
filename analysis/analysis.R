library(ggplot2)
tot = read.table("~/Documents/study/year2/EOS/ProjectEoS/code/results.txt", quote="\"", comment.char="")
colnames(tot) = c("iteration", "agent", "size", "signal", "success", "used", "F1", "F2")
tot = tot[tot$iteration == max(tot$iteration),]
#tot = tot[tot$agent == 1,]

ggplot(tot, aes(x=iteration, y=success)) +
  facet_wrap(~ signal) +
  geom_line()

ggplot(tot, aes(x=F2, y=F1)) +
  facet_wrap(~ agent) +
  geom_point() +
  labs(x=expression("F'"[2] (Bark)), y=expression("F"[1] (Bark)), title=paste(as.character(max(tot$iteration)+1),"Games"))
