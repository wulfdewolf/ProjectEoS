library(ggplot2)
library(igraph)
library(ggnet)
library(network)

# Get agents' repertoires
data = read.table("~/Documents/study/year2/EOS/ProjectEoS/code/results.txt", quote="\"", comment.char="")
colnames(data) = c("iteration", "agent", "size", "signal", "success", "used", "F1", "F2")
tot = data[data$iteration == 9999,]
#tot$agent = tot$agent+1

# Get agents' edges
edges = read.table("~/Documents/study/year2/EOS/ProjectEoS/code/network.txt", quote="\"", comment.char="")
edges = edges[-1,]

# Combined repertoires
ggplot(tot, aes(x=F2, y=F1)) +
  scale_y_reverse() +
  scale_x_reverse() +
  geom_point() +
  labs(x=expression("F'"[2] (Bark)), y=expression("F"[1] (Bark)), title=paste(as.character(max(tot$iteration)+1),"Games"))
ggsave("analysis/plots/FC_10000.pdf", device="pdf")

# Separated repertoires
agent_labeller = function(variable, value) {
  return(paste("Agent ", as.character(value+1), " (", as.character(table(edges[1])[value]), ")"))
}
ggplot(tot, aes(x=F2, y=F1)) +
  facet_wrap(~ agent, labeller=agent_labeller) +
  scale_y_reverse() +
  scale_x_reverse() +
  geom_point() +
  labs(x=expression("F'"[2] (Bark)), y=expression("F"[1] (Bark)), title=paste(as.character(max(tot$iteration)+1),"Games"))
ggsave("analysis/plots/BA_80000_split.pdf", device="pdf")

# Network
net = as.matrix(get.adjacency(graph.data.frame(edges)))
network_plot = network(net)
ggnet2(network_plot, label=TRUE, mode="circle") +
  guides(color = FALSE, size = FALSE)
ggsave("analysis/plots/netBA.pdf", device="pdf")

# Degree distribution
net.degrees <- degree(net)
net.degree.histogram <- as.data.frame(table(net.degrees))
net.degree.histogram[,1] <- as.numeric(net.degree.histogram[,1])
ggplot(net.degree.histogram, aes(x = net.degrees, y = Freq)) +
  geom_point() +
  scale_x_continuous("Degree (log)\n",
                     breaks = c(1, 3, 10, 30, 100, 300),
                     trans = "log10") +
  scale_y_continuous("Frequency (log)\n",
                     breaks = c(1, 3, 10, 30, 100, 300, 1000),
                     trans = "log10") +
  theme_bw()
ggsave("analysis/plots/net_degree.pdf", device="pdf")

# Repertoire sizes
ggplot(aggregate(. ~ agent, data = tot, FUN = tail, 1), aes(x=as.factor(agent), y=size)) +
  geom_bar(stat="identity") +
  labs(x="Agent", y="Repertoire size")
