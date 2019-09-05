library(deldir)
library(ggplot2)

args = commandArgs(trailingOnly=TRUE)

if (length(args)==0) {
	stop("Missing argument for input file.", call.=FALSE)
}

r = read.table(file=args[1], header=FALSE, skip=2, col.names=c("fcv","x","y","cls"))
r$cls = factor(r$cls)

plot <- ggplot(data=r, aes(x=x, y=y)) +
  #Plot the points
  geom_point(size=2, aes(colour=cls)) +
  coord_cartesian(xlim=c(0,1), ylim=c(0,1), expand=c(0,0)) + 
  theme_void() + 
  theme(
    plot.margin = margin(0.1,0.1,0.1,0.1,unit="cm"),
    legend.position="none")

ggsave("img/tmp.png", device="png", width=25, height=25, units="cm")
