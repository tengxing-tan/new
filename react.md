# Props

**Good practices**
Inherit properties from React component.

```jsx
class ToDo extends React.Component {
    construstor(props) {
        super(props);
    }
    
}
```

**Default properties**

```jsx
const toDoList = (props) => {
    return props.tasks;
}

// let say your component is ToDO
class ToDo extends React.Component {
    // something inside
}

// set default props value
ToDo.defaultProps = { // declare object
    tasks: 'What to do today...'
};
```