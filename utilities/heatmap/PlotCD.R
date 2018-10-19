library(ggplot2)
library(viridis)

data <- read.csv("data/tmp.csv", header = TRUE)
data$c <- pmin(data$cd, 0.5)
size <- sqrt(length(data$cd))

p <- ggplot(data, aes(x, y)) +
  geom_raster(aes(fill = c)) +
  scale_x_continuous(limits=c(1,size), expand=c(0,0)) +
  scale_y_continuous(limits=c(1,size), expand=c(0,0)) +
  theme_void() +
  theme(legend.position="none") + 
  scale_fill_viridis()

ggsave("img/tmp.png", device=png(), width=30, height=30, units="cm", dpi=72)
