library(ggplot2)
tot = read.table("~/Documents/study/year2/EOS/ProjectEoS/code/results.txt", quote="\"", comment.char="")
colnames(tot) = c("iteration", "agent", "size", "signal", "success", "used", "F1", "F2")
tot = tot[tot$iteration == max(tot$iteration),]
tot$agent = tot$agent+1
#tot = tot[tot$agent == 1,]

# Combined repertoires
ggplot(tot, aes(x=F2, y=F1)) +
  scale_y_reverse() +
  scale_x_reverse() +
  geom_point() +
  labs(x=expression("F'"[2] (Bark)), y=expression("F"[1] (Bark)), title=paste(as.character(max(tot$iteration)+1),"Games"))

ggplot(tot, aes(x=iteration, y=success)) +
  facet_wrap(~ signal) +
  geom_line()

# Separated repertoires
ggplot(tot, aes(x=F2, y=F1)) +
  facet_wrap(~ agent) +
  scale_y_reverse() +
  scale_x_reverse() +
  geom_point() +
  labs(x=expression("F'"[2] (Bark)), y=expression("F"[1] (Bark)), title=paste(as.character(max(tot$iteration)+1),"Games"))


# Repertoire sizes
ggplot(aggregate(. ~ agent, data = tot, FUN = tail, 1), aes(x=as.factor(agent), y=size)) +
  geom_bar(stat="identity") +
  labs(x="Agent", y="Repertoire size")
