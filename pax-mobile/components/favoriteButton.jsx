import React from "react";
import { Pressable, Text, StyleSheet } from "react-native";
import { useTheme } from "../theme/ThemeContext";

const FavoriteButton = ({ isFavorite, onToggle }) => {
  const { theme } = useTheme();
  const styles = createStyles(theme);

  return (
    <Pressable
      onPress={onToggle}
      style={styles.button}
      accessible={true}
      accessibilityRole="button"
      accessibilityLabel={
        isFavorite ? "Ta bort som favorit" : "Lägg till som favorit"
      }
      accessibilityState={{ selected: isFavorite }}
    >
      <Text style={styles.buttonText}>
        {isFavorite ? "★ Ta bort som favorit" : "☆ Lägg till favorit"}
      </Text>
    </Pressable>
  );
};

const createStyles = (theme) =>
  StyleSheet.create({
    button: {
      backgroundColor: theme.textSecondary,
      padding: 10,
      borderRadius: 10,
      marginTop: 10,
      alignItems: "center",
    },
    buttonText: {
      color: "#000",
      fontWeight: "600",
    },
  });

export default FavoriteButton;
