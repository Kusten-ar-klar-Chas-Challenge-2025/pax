import React from "react";
import {
  View,
  Text,
  StyleSheet,
  ScrollView,
  StatusBar,
  Platform,
  Pressable,
  Image,
} from "react-native";
import { SafeAreaView, SafeAreaProvider } from 'react-native-safe-area-context';
import { MaterialCommunityIcons } from "@expo/vector-icons";
import { useTheme } from "../theme/ThemeContext";
import CardComponent from "../components/CardComponent";

const HomeScreen = () => {
  const { theme, isDark, toggleTheme } = useTheme();
  const styles = createStyles(theme);

  //cmt
  return (
    <SafeAreaProvider>
      <ScrollView>
    <SafeAreaView style={styles.safeArea}>
              <View style={styles.header}>
      <StatusBar
        barStyle={isDark ? "light-content" : "dark-content"}
        backgroundColor={theme.headerBackground}
      />
        <Pressable
          onPress={toggleTheme}
          accessible={true}
          accessibilityRole="button"
          accessibilityLabel={
            isDark ? "Byt till ljust tema" : "Byt till mörkt tema"
          }
          style={styles.toggleButton}
        >
          <MaterialCommunityIcons
            name={isDark ? "white-balance-sunny" : "weather-night"}
            size={30}
            color={theme.iconSwitchmode}
          />
        </Pressable>
              <Image
      source={require("../assets/logo/pax_logo-text-10.png")}
      style={styles.logo}
      resizeMode="cover"
    />
          </View>
                <View style={styles.container}>
        <CardComponent style={styles.CardComponentContainer} />
              <Image
      source={require("../assets/illustrationer/illustrationer_pax-27.svg")}
      style={styles.image}
      resizeMode="cover"
    />
      </View>
    </SafeAreaView>
    </ScrollView>
    </SafeAreaProvider>
  );
};

const createStyles = (theme) =>
  StyleSheet.create({
    safeArea: {
      flex: 1,
      backgroundColor: theme.background,
      minHeight: "100vh",
      paddingBottom: 2,
    },
    header: {
      flex: 1,
      flexDirection: "row",
      justifyContent: "center",
      alignItems: "center",
    },
    container: {
      flex: 1,
      backgroundColor: theme.background,
      justifyContent: "flex-start",
      alignItems: "center",
      marginTop: "10vh",
      minHeight: "90vh",
    },
    logo: {
      top: 5,
      marginTop: "4vh",
      width: "40vw",
      height: "8vh",
      padding: 10,
    },
    text: {
      fontSize: 18,
      color: theme.text,
    },
      image: {
    width: "90vw",
    height: "30vh",
    opacity: 0.75,
  },
    toggleButton: {
      position: "absolute",
      top: 20,
      left: 20,
      zIndex: 1,
      backgroundColor: "#E3FAC825",
      borderRadius: 25,
      padding: 5,
    },
    CardComponentContainer: {
      marginTop: "20%",
      bottom: 5,
      alignSelf: "center",
      width: "50vw",
    },
  });

export default HomeScreen;
