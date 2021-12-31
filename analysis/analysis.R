library(ggplot2)
library(igraph)
library(ggnet)
library(network)
library(dgof)

# Repertoires
repertoires <- read.table("~/Documents/study/year2/EOS/ProjectEoS/results/exp2/50/repertoires_BAresults50.txt", quote = "\"", comment.char = "")
colnames(repertoires) <- c("iteration", "agent", "size", "signal", "success", "used", "F1", "F2")
repertoires$agent <- repertoires$agent + 1

# Network
edges <- read.table("~/Documents/study/year2/EOS/ProjectEoS/results/exp1/FCnetwork.txt", quote = "\"", comment.char = "")
edges <- edges[-1, ]

# Statistics
statistics <- read.table("~/Documents/study/year2/EOS/ProjectEoS/results/exp2/100/statistics_BAresults100.txt", quote = "\"", comment.char = "")
colnames(statistics) <- c("success", "size", "energy")

#----------------------------------------------------------------------------------------------------------------------------
# Combined repertoires

for (i in unique(repertoires$iteration)) {
  subset <- repertoires[repertoires$iteration == i, ]
  ggplot(subset, aes(x = F2, y = F1)) +
    scale_y_reverse() +
    scale_x_reverse() +
    geom_point() +
    labs(x = expression("F'"[2](Bark)), y = expression("F"[1](Bark)), title = paste(as.character(i), "Games")) +
    theme(text = element_text(size = 20))
  ggsave(paste("analysis/plots/exp2/50/", as.character(i), ".pdf", sep = ""), width = 7, height = 7, device = "pdf")
}


#----------------------------------------------------------------------------------------------------------------------------
# Separated repertoires

agent_labeller <- function(variable, value) {
  return(paste("Agent ", as.character(value + 1), " (", as.character(table(edges[1])[value]), ")"))
}
ggplot(repertoires, aes(x = F2, y = F1)) +
  facet_wrap(~agent, labeller = agent_labeller) +
  scale_y_reverse() +
  scale_x_reverse() +
  geom_point() +
  labs(x = expression("F'"[2](Bark)), y = expression("F"[1](Bark)), title = paste(as.character(max(repertoires$iteration)), "Games"))
ggsave("analysis/plots/BA_80000_split.pdf", device = "pdf")

#----------------------------------------------------------------------------------------------------------------------------
# Network

net <- as.matrix(get.adjacency(graph.data.frame(edges)))
network_plot <- network(net)
ggnet2(network_plot, label = TRUE, mode = "circle") +
  guides(color = FALSE, size = FALSE)
ggsave("analysis/plots/netBA.pdf", device = "pdf")

# Degree distribution
net.degrees <- degree(net)
net.degree.histogram <- as.data.frame(table(net.degrees))
net.degree.histogram[, 1] <- as.numeric(net.degree.histogram[, 1])
ggplot(net.degree.histogram, aes(x = net.degrees, y = Freq)) +
  geom_point() +
  scale_x_continuous("Degree (log)\n",
    breaks = c(1, 3, 10, 30, 100, 300),
    trans = "log10"
  ) +
  scale_y_continuous("Frequency (log)\n",
    breaks = c(1, 3, 10, 30, 100, 300, 1000),
    trans = "log10"
  ) +
  theme_bw()
ggsave("analysis/plots/net_degree.pdf", device = "pdf")


#----------------------------------------------------------------------------------------------------------------------------
# Success distribution

ggplot(statistics, aes(x = success)) +
  geom_histogram(binwidth = .005, colour = "black", fill = "white") +
  labs(x = "", y = "", title = paste("Success,", as.character(max(repertoires$iteration)), "Games")) +
  theme(text = element_text(size = 20))
ggsave("analysis/plots/exp1/BA/success_distribution.pdf", width = 7, height = 7, device = "pdf")

mean(statistics$success)
sd(statistics$success)

# Kolmogorov-Smirnov test
success20 <- read.table("~/Documents/study/year2/EOS/ProjectEoS/results/exp1/statistics_BAresults.txt", quote = "\"", comment.char = "")[, 1]
success50 <- read.table("~/Documents/study/year2/EOS/ProjectEoS/results/exp2/50/statistics_BAresults50.txt", quote = "\"", comment.char = "")[, 1]
success100 <- read.table("~/Documents/study/year2/EOS/ProjectEoS/results/exp2/100/statistics_BAresults100.txt", quote = "\"", comment.char = "")[, 1]

# 50 vs 20
ks.test(success20, success50, alternative = "l")

# 100 vs 50
ks.test(success50, success100, alternative = "l")


#----------------------------------------------------------------------------------------------------------------------------
# Size distribution

ggplot(statistics, aes(x = size)) +
  geom_histogram(binwidth = .1, colour = "black", fill = "white") +
  labs(x = "", y = "", title = paste("Size,", as.character(max(repertoires$iteration)), "Games")) +
  theme(text = element_text(size = 20))
ggsave("analysis/plots/exp1/BA/size_distribution.pdf", width = 7, height = 7, device = "pdf")

mean(statistics$size)
sd(statistics$size)

#----------------------------------------------------------------------------------------------------------------------------
# Energy distribution

ggplot(statistics, aes(x = as.numeric(energy))) +
  geom_histogram(binwidth = 0.1, colour = "black", fill = "white") +
  labs(x = "", y = "", title = paste("Energy,", as.character(max(repertoires$iteration)), "Games")) +
  theme(text = element_text(size = 20))
ggsave("analysis/plots/exp1/BA/energy_distribution.pdf", width = 7, height = 7, device = "pdf")

mean(statistics$energy)
sd(statistics$energy)

# Kolmogorov-Smirnov test
energy20 <- read.table("~/Documents/study/year2/EOS/ProjectEoS/results/exp1/statistics_BAresults.txt", quote = "\"", comment.char = "")[, 2]
energy50 <- read.table("~/Documents/study/year2/EOS/ProjectEoS/results/exp2/50/statistics_BAresults50.txt", quote = "\"", comment.char = "")[, 2]
energy100 <- read.table("~/Documents/study/year2/EOS/ProjectEoS/results/exp2/100/statistics_BAresults100.txt", quote = "\"", comment.char = "")[, 2]

# 50 vs 20
ks.test(energy20, energy50, alternative = "g")

# 100 vs 50
ks.test(energy50, energy100, alternative = "l")
