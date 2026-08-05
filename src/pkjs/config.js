module.exports = [
  {
    type: 'heading',
    defaultValue: 'Watchface Settings',
  },
  {
    type: 'section',
    items: [
      {
        type: 'radiogroup',
        messageKey: 'CATPPUCCIN_FLAVOR',
        label: 'Flavor',
        defaultValue: 'mocha',
        options: [
          { label: 'Latte', value: 'latte' },
          { label: 'Frappe', value: 'frappe' },
          { label: 'Macchiato', value: 'macchiato' },
          { label: 'Mocha', value: 'mocha' },
        ],
      },
      {
        type: 'toggle',
        messageKey: 'SHOW_HEARTRATE',
        label: 'Show Heart Rate',
        defaultValue: true,
        description: 'Show heart icon + BPM in the top-left corner'
      },
      {
        type: 'radiogroup',
        messageKey: 'SHOW_HEARTRATE_MODE',
        label: 'Heart Rate Mode',
        defaultValue: '0',
        options: [
          { label: 'Passive (peek + subscribe)', value: '0' },
          { label: 'Live (continuous sampling)', value: '1' }
        ],
        description: 'Passive = OS-provided updates. Live = continuous sampling (higher battery use).',
        required: [
          { key: 'SHOW_HEARTRATE', value: true }
        ]
      },
      {
        type: 'toggle',
        messageKey: 'SHOW_STEPS',
        label: 'Show Steps',
        defaultValue: true,
        description: 'Show walking icon + current step count at the bottom (centered).'
      },
    ],
  },
  {
    type: 'submit',
    defaultValue: 'Save Settings',
  },
];
