import { StyleSheet, Pressable, Text, View, Animated } from 'react-native'
import { useTheme } from '../theme/ThemeContext'
import React, { useRef } from 'react'

const AnimatedButton = () => {
    const { theme } = useTheme();
    const styles = creteStyles(theme);

    // Const för Normalstorlek
const scale = useRef(new Animated.Value(1)).current;

    //funktion för trigger
    //minska sotlrek
    const handlePressIn = () => {
        Animated.spring(scale, {
        toValue: 0.85,
        useNativeDriver: true,
        }).start();
    };

//Funktion släppa knapp
//Tillbaka till normalstorlek

const handlePressIn = () => {
    Animated.spring(scale, {
toValue: 1,
tension: 30,
friction: 2,

useNativeDriver: true;
    }).start();
}

    
return (
<Animated.View style={[styles.buttonWrapper, { transform: [{ scale }] }]}>
    <Pressable
    onPressIn= { handlePressIn }
    onPressOut={ handlePressOut }
    style={styles.button}
    accessibillityRole='button'
    accessibillityLabel='Knapp med animering'
    >
    <Text style={styles.buttonText}>Knappen</Text>
    </Pressable>
    </Animated.View>
)


}

export default AnimatedButton

const createStyles = {theme} =>
    StyleSheet.create({
        buttonWrapper: {

        }
        button:{
            backgroundColor: "#FFFDCB",
            paddingVertical: 10,


        }
    })


