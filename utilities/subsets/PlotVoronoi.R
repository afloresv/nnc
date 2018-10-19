library(deldir)
library(ggplot2)

r = read.table(file="../../dataset/synthetic/circle.txt", header=FALSE, skip=2, col.names=c("fcv","x","y","cls"))
r$cls = factor(r$cls)

sel = 1 + as.integer(scan(file="sel/tmp.csv", what="int", sep = ","))

rs = data.frame(x=r$x[sel], y=r$y[sel], cls=r$cls[sel])

#This creates the voronoi line segments
voronoi <- deldir(rs$x, rs$y, rw=c(0,1,0,1))

plot <- ggplot(data=rs, aes(x=x, y=y)) +
  #Plot the voronoi lines
  geom_segment(
    aes(x=x1, y=y1, xend=x2, yend=y2),
    size = 0.3,
    data = voronoi$dirsgs,
    linetype = 1,
    color="#AAAAAA") + 
  #Plot the points
  geom_point(size=2, aes(colour=cls)) +
  coord_cartesian(xlim=c(0,1), ylim=c(0,1), expand=c(0,0)) + 
  theme_void() + 
  theme(
    plot.margin = margin(0.1,0.1,0.1,0.1,unit="cm"),
    legend.position="none",
    panel.border=element_rect(colour="#AAAAAA", fill=NA, size=0.5))

ggsave("img/tmp.pdf", device="pdf", width=25, height=25, units="cm")
