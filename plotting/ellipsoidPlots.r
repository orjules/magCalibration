data <- read.csv("./plotting/ellipsoidOut.txt",
    header = TRUE, sep = ",", dec = ".")

columns <- c("x0", "y0", "z0", "a", "b", "c")

png(
    filename = paste("./images/ellipsoidPlots/",
        data[1, "learningRate"], ".png", sep = "")
)

par(mfrow = c(2, 3))
for (i in 1: 6) {
    plot(
        data$"step",
        data[, i+1],
        "l",
        xlab = "Steps",
        ylab = columns[i],
        main = columns[i]
    )
}

mtext(sprintf("Gradient descent with %1.13f", data[1, "learningRate"])
      , side = 3, line = -1.5, outer = TRUE)

dev.off()